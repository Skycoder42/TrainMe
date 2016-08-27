#include "traindatamanager.h"
#include <QtSql>
#include <QStandardPaths>
#include <QDir>
#include <QtConcurrent>

#include <QDebug>

TrainDataManager::TrainDataManager(QObject *parent) :
	QObject(parent),
	database(QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"))),
	dbThread(new QThreadPool(this))
{
	this->dbThread->setMaxThreadCount(1);
	//DEBUG
	connect(this, &TrainDataManager::managerError, this, [](QString error) {
		qDebug() << error;
	});
}

TrainDataManager::~TrainDataManager()
{
	this->database.close();
	this->database = QSqlDatabase();
}

QString TrainDataManager::trResult(int taskResult)
{
	switch (taskResult) {
	case TrainDataManager::Done:
		return tr("Done");
	case TrainDataManager::Skip:
		return tr("Skipped");
	case TrainDataManager::Sick:
		return tr("Sick");
	case TrainDataManager::Gain:
		return tr("Other Training");
	case TrainDataManager::Fail:
		return tr("Failed");
	case TrainDataManager::Free:
		return tr("Free Day");
	case TrainDataManager::Unknown:
		return tr("<font color=\"#FF0000\">Missing</font>");
	default:
		Q_UNREACHABLE();
	}
}

void TrainDataManager::initManager()
{
	QtConcurrent::run(this->dbThread, [=](){
		const QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
		dir.mkpath(QStringLiteral("."));
		const auto dbName = dir.absoluteFilePath(QStringLiteral("train.db"));
#ifndef QT_NO_DEBUG
		qDebug() << "db replaced:" << QFile::remove(dbName);
#endif
		if(QFile::copy(QStringLiteral(":/train_struct.sqlite"), dbName))
			QFile::setPermissions(dbName, QFileDevice::ReadUser | QFileDevice::WriteUser);

		this->database.setDatabaseName(dbName);
		if(this->database.isValid() &&
		   this->database.open()) {
			auto initIndex = 0;

			if(this->testHasMissingDates())
				initIndex = 1;

			emit managerReady(initIndex);//DEBUG
		} else
			emit managerError(tr("Unable to load database from \"%1\"").arg(dbName), true);
	});
}

void TrainDataManager::loadTrainingAllowed()
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("SELECT * FROM TaskCalendar "
									 "WHERE Date = ?"));
		query.bindValue(0, QDate::currentDate());
		if(query.exec())
			emit traingAllowedLoaded(!query.first());
		else
			emit managerError(query.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::loadAllTasks()
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		int penalty;
		bool forAgility;
		QSqlQuery penaltyQuery(this->database);
		penaltyQuery.prepare(QStringLiteral("SELECT PenaltyFactor, PenaltyCount, AgilityPenalties FROM Meta"));
		if(penaltyQuery.exec() && penaltyQuery.first()) {
			penalty = qRound(penaltyQuery.record().value(QStringLiteral("PenaltyFactor")).toDouble() *
							 penaltyQuery.record().value(QStringLiteral("PenaltyCount")).toInt());
			forAgility = penaltyQuery.record().value(QStringLiteral("AgilityPenalties")).toBool();
		} else {
			emit managerError(penaltyQuery.lastError().text(), true);
			emit operationCompleted();
			return;
		}

		QSqlQuery strengthQuery(this->database);
		strengthQuery.prepare(QStringLiteral("SELECT * FROM StrengthTasks"));
		if(strengthQuery.exec()) {
			QList<QSharedPointer<TrainTask>> resList;
			while(strengthQuery.next()) {
				auto task = new TrainTask(strengthQuery.record().value(QStringLiteral("Name")).toString(),
										  TrainTask::StrengthTask,
										  strengthQuery.record().value(QStringLiteral("BaseCount")).toInt(),
										  strengthQuery.record().value(QStringLiteral("Factor")).toDouble(),
										  strengthQuery.record().value(QStringLiteral("Increment")).toInt(),
										  penalty);
				resList.append(QSharedPointer<TrainTask>(task));
			}
			emit tasksLoaded(TrainTask::StrengthTask, resList);
		} else
			emit managerError(strengthQuery.lastError().text(), true);

		QSqlQuery agilityQuery(this->database);
		agilityQuery.prepare(QStringLiteral("SELECT * FROM AgilityTasks"));
		if(agilityQuery.exec()) {
			QList<QSharedPointer<TrainTask>> resList;
			while(agilityQuery.next()) {
				auto task = new TrainTask(agilityQuery.record().value(QStringLiteral("Name")).toString(),
										  TrainTask::AgilityTask,
										  agilityQuery.record().value(QStringLiteral("Count")).toInt(),
										  1.0,
										  0,
										  forAgility ? penalty : 0.0);
				resList.append(QSharedPointer<TrainTask>(task));
			}
			emit tasksLoaded(TrainTask::AgilityTask, resList);
		} else
			emit managerError(agilityQuery.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::loadTaskResults(bool fillDates)
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("SELECT * FROM TaskCalendar "
									 "ORDER BY Date ASC"));
		if(query.exec()) {
			QList<ResultInfo> resList;
			while(query.next()) {
				resList.append({
								   query.record().value(QStringLiteral("Date")).toDate(),
								   (TaskResult)query.record().value(QStringLiteral("Result")).toInt()
							   });
			}

			if(fillDates && !resList.isEmpty()) {
				QDate nextDate = resList.first().first;
				for(int i = 1; i < resList.size(); i++) {
					nextDate = nextDate.addDays(1);
					if(resList[i].first != nextDate)
						resList.insert(i, {nextDate, Unknown});
				}
			}

			emit taskResultsLoaded(resList);
		} else
			emit managerError(query.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::loadWeekConfig()
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery loadTrainMapQuery(this->database);
		loadTrainMapQuery.prepare(QStringLiteral("SELECT * FROM TrainMap"));
		if(loadTrainMapQuery.exec()) {
			Weekonfig config;
			while(loadTrainMapQuery.next()) {
				config.insert((Qt::DayOfWeek)loadTrainMapQuery.record().value(QStringLiteral("Weekday")).toInt(),
							  {
								  loadTrainMapQuery.record().value(QStringLiteral("Increase")).toInt(),
								  loadTrainMapQuery.record().value(QStringLiteral("AddTask")).toBool()
							  });
			}
			emit weekConfigLoaded(config);
		} else
			emit managerError(loadTrainMapQuery.lastError().text(), true);

		QSqlQuery loadExtraQuery(this->database);
		loadExtraQuery.prepare(QStringLiteral("SELECT PenaltyFactor, MaxFreeDays, AgilityPenalties "
											  "FROM Meta"));
		if(loadExtraQuery.exec() && loadExtraQuery.first()) {
			emit configExtrasLoaded(loadExtraQuery.record().value(QStringLiteral("PenaltyFactor")).toDouble(),
									loadExtraQuery.record().value(QStringLiteral("MaxFreeDays")).toInt(),
									loadExtraQuery.record().value(QStringLiteral("AgilityPenalties")).toBool());
		} else
			emit managerError(loadExtraQuery.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::updateWeekConfigIncrement(Qt::DayOfWeek dayOfWeek, int increment)
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE TrainMap "
									 "SET Increase = ? "
									 "WHERE Weekday = ?"));
		query.bindValue(0, increment);
		query.bindValue(1, dayOfWeek);
		if(!query.exec())
			emit managerError(query.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::updateWeekConfigAddTask(Qt::DayOfWeek dayOfWeek, bool addTask)
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE TrainMap "
									 "SET AddTask = ? "
									 "WHERE Weekday = ?"));
		query.bindValue(0, addTask);
		query.bindValue(1, dayOfWeek);
		if(!query.exec())
			emit managerError(query.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::updatePenaltyFactor(double penaltyFactor)
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE Meta SET PenaltyFactor = ?"));
		query.bindValue(0, penaltyFactor);
		if(!query.exec())
			emit managerError(query.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::updateMaxFreeDays(int maxFreeDays)
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE Meta SET MaxFreeDays = ?"));
		query.bindValue(0, maxFreeDays);
		if(!query.exec())
			emit managerError(query.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::updateAgilityPenalties(bool agilityPenalties)
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE Meta SET AgilityPenalties = ?"));
		query.bindValue(0, agilityPenalties);
		if(!query.exec())
			emit managerError(query.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::completeTasks(const QDate &date, TrainDataManager::TaskResult result)
{
	QtConcurrent::run(this->dbThread, [=]() {
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("INSERT INTO TaskCalendar "
									 "(Date, Result) "
									 "VALUES(?, ?)"));
		query.bindValue(0, date);
		query.bindValue(1, (int)result);

		if(!query.exec())
			emit managerError(query.lastError().text(), false);
		else {
			switch(result) {
			case Done:
				this->recalcScores(date);
				this->resetPenalty();
				break;
			case Skip:
				this->addPenalty();
				break;
			case Sick:
				this->resetPenalty();
				break;
			case Gain:
				break;
			case Fail:
				this->resetPenalty(true);
				break;
			case Free:
			{
				auto leftFreeDays = this->addFree(date);
				if(leftFreeDays >= 0)
					emit managerError(tr("Free days left: %L1").arg(leftFreeDays), false, tr("Free Days"));
				else {
					this->addPenalty(-leftFreeDays);
					emit managerError(tr("You have %L1 more free days then allowed! This will add penalty!").arg(-leftFreeDays), false, tr("Free Days"));
				}
				break;
			}
			default:
				Q_UNREACHABLE();
			}
		}

		emit operationCompleted();
	});
}

void TrainDataManager::recalcScores(const QDate &date)
{
	//update score and task count based on day
	QSqlQuery dayInfoQuery(this->database);
	dayInfoQuery.prepare(QStringLiteral("SELECT * FROM TrainMap "
										"WHERE Weekday = ?"));
	dayInfoQuery.bindValue(0, date.addDays(1).dayOfWeek());
	if(!dayInfoQuery.exec())
		emit managerError(dayInfoQuery.lastError().text(), false);
	else if(dayInfoQuery.first()) {
		auto increase = dayInfoQuery.value(QStringLiteral("Increase")).toInt();
		if(increase > 0) {
			QSqlQuery updateScoreQuery(this->database);
			updateScoreQuery.prepare(QStringLiteral("UPDATE Meta SET Score = Score + ?"));
			updateScoreQuery.bindValue(0, increase);
			if(!updateScoreQuery.exec())
				emit managerError(updateScoreQuery.lastError().text(), false);
		}

		if(dayInfoQuery.value(QStringLiteral("AddTask")).toBool()) {
			QSqlQuery addTaskQuery(this->database);
			addTaskQuery.prepare(QStringLiteral("UPDATE Meta SET TaskCount = TaskCount + 1"));
			if(!addTaskQuery.exec())
				emit managerError(addTaskQuery.lastError().text(), false);
		}
	}

	//update all to target score
	QSqlQuery loadScoreQuery(this->database);
	loadScoreQuery.prepare(QStringLiteral("SELECT Score FROM Meta"));
	if(loadScoreQuery.exec() && loadScoreQuery.first()) {
		auto score = loadScoreQuery.record().value(QStringLiteral("Score")).toInt();
		QSqlQuery updateTasksQuery(this->database);
		updateTasksQuery.prepare(QStringLiteral("UPDATE StrengthTasks SET Increment = Increment + 1 "
												"WHERE Increment < ?"));
		updateTasksQuery.bindValue(0, score);
		if(!updateTasksQuery.exec())
			emit managerError(updateTasksQuery.lastError().text(), false);
	} else
		emit managerError(loadScoreQuery.lastError().text(), false);
}

void TrainDataManager::addPenalty(int amount)
{
	QSqlQuery query(this->database);
	query.prepare(QStringLiteral("UPDATE Meta SET PenaltyCount = PenaltyCount + ?"));
	query.bindValue(0, amount);
	if(!query.exec())
		emit managerError(query.lastError().text(), false);
}

int TrainDataManager::addFree(const QDate &date)//TODO reset every year
{
	QSqlQuery updateQuery(this->database);
	updateQuery.prepare(QStringLiteral("INSERT OR REPLACE INTO FreeDays (Year, FreeDays) "
									   "VALUES(:year,  COALESCE(("
									   "  SELECT FreeDays FROM FreeDays"
									   "  WHERE Year = :year"
									   "), 0) + 1);"));
	updateQuery.bindValue(QStringLiteral(":year"), date.year());
	if(!updateQuery.exec()) {
		emit managerError(updateQuery.lastError().text(), false);
		return 0;
	}


	QSqlQuery statusQuery(this->database);
	statusQuery.prepare(QStringLiteral("SELECT ("
									   "  (SELECT MaxFreeDays FROM Meta) - "
									   "  (SELECT FreeDays FROM FreeDays WHERE Year = ?)"
									   ") AS Result"));
	statusQuery.bindValue(0, date.year());
	if(statusQuery.exec() && statusQuery.first())
		return statusQuery.record().value(QStringLiteral("Result")).toInt();
	else {
		emit managerError(statusQuery.lastError().text(), false);
		return 0;
	}
}

void TrainDataManager::resetPenalty(bool reduceOnly)
{
	QSqlQuery query(this->database);
	if(reduceOnly)
		query.prepare(QStringLiteral("UPDATE Meta SET PenaltyCount = max(PenaltyCount - 1, 0)"));
	else
		query.prepare(QStringLiteral("UPDATE Meta SET PenaltyCount = 0"));
	if(!query.exec())
		emit managerError(query.lastError().text(), false);
}

bool TrainDataManager::testHasMissingDates()
{
	QSqlQuery query(this->database);
	query.prepare(QStringLiteral("SELECT Date FROM TaskCalendar "
								 "ORDER BY Date ASC"));
	if(query.exec()) {
		if(query.first()) {
			QDate nextDate = query.record().value(QStringLiteral("Date")).toDate();
			while(query.next()) {
				nextDate = nextDate.addDays(1);
				auto date = query.record().value(QStringLiteral("Date")).toDate();
				if(date != nextDate)
					return true;
			}
		}
	} else
		emit managerError(query.lastError().text(), true);

	return false;
}
