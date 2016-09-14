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
//#ifndef QT_NO_DEBUG
//		qDebug() << "db replaced:" << QFile::remove(dbName);
//#endif
		if(QFile::copy(QStringLiteral(":/train_struct.sqlite"), dbName))
			QFile::setPermissions(dbName, QFileDevice::ReadUser | QFileDevice::WriteUser);

		this->database.setDatabaseName(dbName);
		if(this->database.isValid() &&
		   this->database.open()) {
			auto initIndex = 0;

			if(this->testHasMissingDates())
				initIndex = 1;
			else if (this->loadMissingTasks())
				initIndex = 2;

			emit managerReady(5);//DEBUG
		} else {
			emit managerMessage(tr("Fatal Error!"),
								tr("Unable to load database from \"%1\"").arg(dbName),
								true);
		}
	});
}

void TrainDataManager::loadTrainingAllowed()
{
	this->executeAsync([=](){
		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("SELECT * FROM TaskCalendar "
									 "WHERE Date = ?"));
		query.bindValue(0, QDate::currentDate());
		if(query.exec()) {
			if(query.first()) {
				emit traingAllowedLoaded(false);
				emit managerMessage(tr("Training not allowed"),
									tr("You have already trained today! Rest and save your energy for tomorrow!"),
									false);
			} else if(this->testHasMissingTasks()) {
				emit traingAllowedLoaded(false);
				emit managerMessage(tr("Training not allowed"),
									tr("You must add new task before you can start training!"),
									false);
			} else
				emit traingAllowedLoaded(true);
		} else
			throw query.lastError();
	});
}

void TrainDataManager::loadAllTasks()
{
	this->executeAsync([=](){
		int penalty;
		bool forAgility;
		QSqlQuery penaltyQuery(this->database);
		penaltyQuery.prepare(QStringLiteral("SELECT PenaltyFactor, PenaltyCount, AgilityPenalties FROM Meta"));
		if(penaltyQuery.exec() && penaltyQuery.first()) {
			penalty = qRound(penaltyQuery.record().value(QStringLiteral("PenaltyFactor")).toDouble() *
							 penaltyQuery.record().value(QStringLiteral("PenaltyCount")).toInt());
			forAgility = penaltyQuery.record().value(QStringLiteral("AgilityPenalties")).toBool();
		} else
			throw penaltyQuery.lastError();

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
			throw strengthQuery.lastError();

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
			throw agilityQuery.lastError();
	});
}

void TrainDataManager::loadTaskResults(bool fillDates)
{
	this->executeAsync([=](){
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
			throw query.lastError();
	});
}

void TrainDataManager::loadWeekConfig()
{
	this->executeAsync([=](){
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
			throw loadTrainMapQuery.lastError();

		QSqlQuery loadExtraQuery(this->database);
		loadExtraQuery.prepare(QStringLiteral("SELECT PenaltyFactor, MaxFreeDays, AgilityPenalties "
											  "FROM Meta"));
		if(loadExtraQuery.exec() && loadExtraQuery.first()) {
			emit configExtrasLoaded(loadExtraQuery.record().value(QStringLiteral("PenaltyFactor")).toDouble(),
									loadExtraQuery.record().value(QStringLiteral("MaxFreeDays")).toInt(),
									loadExtraQuery.record().value(QStringLiteral("AgilityPenalties")).toBool());
		} else
			throw loadExtraQuery.lastError();
	});
}

void TrainDataManager::updateWeekConfigIncrement(Qt::DayOfWeek dayOfWeek, int increment)
{
	this->executeAsync([=](){
		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE TrainMap "
									 "SET Increase = ? "
									 "WHERE Weekday = ?"));
		query.bindValue(0, increment);
		query.bindValue(1, dayOfWeek);
		if(!query.exec())
			throw query.lastError();
	});
}

void TrainDataManager::updateWeekConfigAddTask(Qt::DayOfWeek dayOfWeek, bool addTask)
{
	this->executeAsync([=](){
		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE TrainMap "
									 "SET AddTask = ? "
									 "WHERE Weekday = ?"));
		query.bindValue(0, addTask);
		query.bindValue(1, dayOfWeek);
		if(!query.exec())
			throw query.lastError();
	});
}

void TrainDataManager::updatePenaltyFactor(double penaltyFactor)
{
	this->executeAsync([=](){
		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE Meta SET PenaltyFactor = ?"));
		query.bindValue(0, penaltyFactor);
		if(!query.exec())
			throw query.lastError();
	});
}

void TrainDataManager::updateMaxFreeDays(int maxFreeDays)
{
	this->executeAsync([=](){
		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE Meta SET MaxFreeDays = ?"));
		query.bindValue(0, maxFreeDays);
		if(!query.exec())
			throw query.lastError();
	});
}

void TrainDataManager::updateAgilityPenalties(bool agilityPenalties)
{
	this->executeAsync([=](){
		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("UPDATE Meta SET AgilityPenalties = ?"));
		query.bindValue(0, agilityPenalties);
		if(!query.exec())
			throw query.lastError();
	});
}

void TrainDataManager::restoreWeekDefaults()
{
	this->executeAsync([=](){
		QSqlQuery queryMap(this->database);
		queryMap.prepare(QStringLiteral("UPDATE TrainMap "
										"SET Increase = ((Weekday % 2 = 1) AND (NOT Weekday = 1)), "
										"AddTask = (Weekday = 1);"));
		if(!queryMap.exec())
			throw queryMap.lastError();

		QSqlQuery queryMeta(this->database);
		queryMeta.prepare(QStringLiteral("UPDATE Meta "
										 "SET AgilityPenalties = 0, "
										 "MaxFreeDays = 25, "
										 "PenaltyFactor = 0.25;"));
		if(queryMeta.exec())
			emit resetDone();
		else
			throw queryMeta.lastError();
	});
}

void TrainDataManager::loadFreeTasks()
{
	this->executeAsync([=](){
		bool isAgilityTask;
		auto count = this->loadMissingTasks(&isAgilityTask);
		emit freeTasksLoaded(count, isAgilityTask);
	});
}

void TrainDataManager::addTask(const QSharedPointer<TrainTask> &task)
{
	this->executeAsync([=](){
		int baseCount;
		double factor;
		task->elements(baseCount, factor);

		QSqlQuery query(this->database);
		if(task->taskType() == TrainTask::StrengthTask) {
			query.prepare(QStringLiteral("INSERT INTO StrengthTasks "
										 "(Name, BaseCount, Factor) "
										 "VALUES(?, ?, ?)"));
			query.bindValue(0, task->name());
			query.bindValue(1, baseCount);
			query.bindValue(2, factor);
		} else {
			query.prepare(QStringLiteral("INSERT INTO AgilityTasks "
										 "(Name, Count) "
										 "VALUES(?, ?)"));
			query.bindValue(0, task->name());
			query.bindValue(1, baseCount);
		}

		if(!query.exec())
			throw query.lastError();
		else {
			bool isAgilityTask;
			auto count = this->loadMissingTasks(&isAgilityTask);
			emit freeTasksLoaded(count, isAgilityTask);
		}
	});
}

void TrainDataManager::completeTasks(const QDate &date, TrainDataManager::TaskResult result)
{
	this->executeAsync([=](){
		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("INSERT INTO TaskCalendar "
									 "(Date, Result) "
									 "VALUES(?, ?)"));
		query.bindValue(0, date);
		query.bindValue(1, (int)result);

		if(!query.exec())
			throw query.lastError();
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
					emit managerMessage(tr("Free Days"), tr("Free days left: %L1").arg(leftFreeDays), false);
				else {
					this->addPenalty(-leftFreeDays);
					emit managerMessage(tr("Free Days"),
										tr("You have %L1 more free days then allowed! This will add penalty!")
										.arg(-leftFreeDays),
										false);
				}
				break;
			}
			default:
				Q_UNREACHABLE();
			}
		}
	});
}

void TrainDataManager::executeAsync(const std::function<void ()> &fn)
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		try {
			fn();
		} catch(QSqlError &error) {
			qCritical(qPrintable(error.text()));
			emit managerMessage(tr("Database Error!"), error.text(), true);//DEBUG what here?
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
		throw dayInfoQuery.lastError();
	else if(dayInfoQuery.first()) {
		auto increase = dayInfoQuery.value(QStringLiteral("Increase")).toInt();
		if(increase > 0) {
			QSqlQuery updateScoreQuery(this->database);
			updateScoreQuery.prepare(QStringLiteral("UPDATE Meta SET Score = Score + ?"));
			updateScoreQuery.bindValue(0, increase);
			if(!updateScoreQuery.exec())
				throw updateScoreQuery.lastError();
		}

		if(dayInfoQuery.value(QStringLiteral("AddTask")).toBool()) {
			QSqlQuery addTaskQuery(this->database);
			addTaskQuery.prepare(QStringLiteral("UPDATE Meta SET TaskCount = TaskCount + 1"));
			if(!addTaskQuery.exec())
				throw addTaskQuery.lastError();
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
			throw updateTasksQuery.lastError();
	} else
		throw loadScoreQuery.lastError();
}

void TrainDataManager::addPenalty(int amount)
{
	QSqlQuery query(this->database);
	query.prepare(QStringLiteral("UPDATE Meta SET PenaltyCount = PenaltyCount + ?"));
	query.bindValue(0, amount);
	if(!query.exec())
		throw query.lastError();
}

int TrainDataManager::addFree(const QDate &date)
{
	QSqlQuery updateQuery(this->database);
	updateQuery.prepare(QStringLiteral("INSERT OR REPLACE INTO FreeDays (Year, FreeDays) "
									   "VALUES(:year,  COALESCE(("
									   "  SELECT FreeDays FROM FreeDays"
									   "  WHERE Year = :year"
									   "), 0) + 1);"));
	updateQuery.bindValue(QStringLiteral(":year"), date.year());
	if(!updateQuery.exec())
		throw updateQuery.lastError();

	QSqlQuery statusQuery(this->database);
	statusQuery.prepare(QStringLiteral("SELECT ("
									   "  (SELECT MaxFreeDays FROM Meta) - "
									   "  (SELECT FreeDays FROM FreeDays WHERE Year = ?)"
									   ") AS Result"));
	statusQuery.bindValue(0, date.year());
	if(statusQuery.exec() && statusQuery.first())
		return statusQuery.record().value(QStringLiteral("Result")).toInt();
	else
		throw statusQuery.lastError();
}

void TrainDataManager::resetPenalty(bool reduceOnly)
{
	QSqlQuery query(this->database);
	if(reduceOnly)
		query.prepare(QStringLiteral("UPDATE Meta SET PenaltyCount = max(PenaltyCount - 1, 0)"));
	else
		query.prepare(QStringLiteral("UPDATE Meta SET PenaltyCount = 0"));
	if(!query.exec())
		throw query.lastError();
}

int TrainDataManager::loadMissingTasks(bool *isAgilityTask)
{
	int taskCount;
	QSqlQuery reqCountQuery(this->database);
	reqCountQuery.prepare(QStringLiteral("SELECT TaskCount FROM Meta"));
	if(reqCountQuery.exec() && reqCountQuery.first())
		taskCount = reqCountQuery.record().value(QStringLiteral("TaskCount")).toInt();
	else
		throw reqCountQuery.lastError();

	int strCount;
	QSqlQuery strCountQuery(this->database);
	strCountQuery.prepare(QStringLiteral("SELECT COUNT(*) AS TaskCount FROM StrengthTasks"));
	if(strCountQuery.exec() && strCountQuery.first())
		strCount = strCountQuery.record().value(QStringLiteral("TaskCount")).toInt();
	else
		throw strCountQuery.lastError();

	int aglCount;
	QSqlQuery aglCountQuery(this->database);
	aglCountQuery.prepare(QStringLiteral("SELECT COUNT(*) AS TaskCount FROM AgilityTasks"));
	if(aglCountQuery.exec() && aglCountQuery.first())
		aglCount = aglCountQuery.record().value(QStringLiteral("TaskCount")).toInt();
	else
		throw aglCountQuery.lastError();

	if(isAgilityTask)
		*isAgilityTask = strCount > aglCount;

	return qMax(taskCount - (strCount + aglCount), 0);
}

bool TrainDataManager::testHasMissingTasks()
{
	return this->loadMissingTasks() > 0;
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
			return false;
		} else
			return false;
	} else
		throw query.lastError();
}
