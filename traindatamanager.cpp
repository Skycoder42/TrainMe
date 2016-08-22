#include "traindatamanager.h"
#include <QtSql>
#include <QStandardPaths>
#include <QDir>
#include <QtConcurrent>

#include <QDebug>

TrainDataManager::TrainDataManager(QObject *parent, const char *initSlot) :
	QObject(parent),
	database(QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"))),
	dbThread(new QThreadPool(this))
{
	this->dbThread->setMaxThreadCount(1);

	if(initSlot) {
		connect(this, SIGNAL(managerReady(QString)),
				parent, initSlot,
				Qt::QueuedConnection);
	}

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
		   this->database.open())
			emit managerReady(QString());
		else
			emit managerReady(tr("Unable to load database from \"%1\"").arg(dbName));
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
		return tr("<font color=\"#FF0000\">Not Set</font>");
	default:
		Q_UNREACHABLE();
	}
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

void TrainDataManager::loadStrengthTasks()
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("SELECT * FROM StrengthTable"));
		if(query.exec()) {
			QList<QSharedPointer<TrainTask>> resList;
			while(query.next()) {
				auto task = new TrainTask(query.record().value(QStringLiteral("Name")).toString(),
										  TrainTask::StrengthTask,
										  query.record().value(QStringLiteral("BaseCount")).toInt(),
										  query.record().value(QStringLiteral("Factor")).toDouble(),
										  query.record().value(QStringLiteral("Increment")).toInt());
				resList.append(QSharedPointer<TrainTask>(task));
			}
			emit tasksLoaded(TrainTask::StrengthTask, resList);
		} else
			emit managerError(query.lastError().text(), true);

		emit operationCompleted();
	});
}

void TrainDataManager::loadAgilityTasks()
{
	QtConcurrent::run(this->dbThread, [=](){
		emit operationStarted();

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("SELECT * FROM AgilityTable"));
		if(query.exec()) {
			QList<QSharedPointer<TrainTask>> resList;
			while(query.next()) {
				auto task = new TrainTask(query.record().value(QStringLiteral("Name")).toString(),
										  TrainTask::AgilityTask,
										  query.record().value(QStringLiteral("Count")).toInt());
				resList.append(QSharedPointer<TrainTask>(task));
			}
			emit tasksLoaded(TrainTask::AgilityTask, resList);
		} else
			emit managerError(query.lastError().text(), true);

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
				break;
			case Skip:
				this->addPenalty();
				break;
			case Sick:
			case Gain:
			case Fail:
				break;
			case Free://TODO limit free day count!
				break;
			default:
				Q_UNREACHABLE();
			}
		}

		emit operationCompleted();
	});
}

void TrainDataManager::recalcScores(const QDate &date)
{
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
		updateTasksQuery.prepare(QStringLiteral("UPDATE StrengthTable SET Increment = Increment + 1 "
												"WHERE Increment < ?"));
		updateTasksQuery.bindValue(0, score);
		if(!updateTasksQuery.exec())
			emit managerError(updateTasksQuery.lastError().text(), false);
	} else
		emit managerError(loadScoreQuery.lastError().text(), false);
}

void TrainDataManager::addPenalty()
{
	QSqlQuery query(this->database);
	query.prepare(QStringLiteral("UPDATE Meta SET PenaltyCount = PenaltyCount + 1"));
	if(!query.exec())
		emit managerError(query.lastError().text(), false);
}
