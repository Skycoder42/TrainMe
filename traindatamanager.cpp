#include "traindatamanager.h"
#include <QtSql>
#include <QStandardPaths>
#include <QDir>
#include <QtConcurrent>

#include <QDebug>

#define W_LOCK QWriteLocker lock(&this->dbLock)
#define R_LOCK QReadLocker lock(&this->dbLock)

TrainDataManager::TrainDataManager(QObject *parent, const char *initSlot) :
	QObject(parent),
	database(QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"))),
	dbLock()
{
	if(initSlot) {
		connect(this, SIGNAL(managerReady(QString)),
				parent, initSlot,
				Qt::QueuedConnection);
	}

	QtConcurrent::run([this](){
		W_LOCK;

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

void TrainDataManager::loadStrengthTasks()
{
	QtConcurrent::run([this](){
		R_LOCK;

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("SELECT * FROM StrengthTable"));
		if(query.exec()) {
			QList<QSharedPointer<TrainTask>> resList;
			while(query.next()) {
				auto task = new TrainTask(query.record().value(QStringLiteral("Name")).toString(),
										  TrainTask::StrengthTask,
										  query.record().value(QStringLiteral("BaseCount")).toInt(),
										  query.record().value(QStringLiteral("Increment")).toInt());
				resList.append(QSharedPointer<TrainTask>(task));
			}
			emit tasksLoaded(TrainTask::StrengthTask, resList);
		} else
			emit managerError(query.lastError().text(), true);
	});
}

void TrainDataManager::loadAgilityTasks()
{
	QtConcurrent::run([this](){
		R_LOCK;

		QSqlQuery query(this->database);
		query.prepare(QStringLiteral("SELECT * FROM AgilityTable"));
		if(query.exec()) {
			QList<QSharedPointer<TrainTask>> resList;
			while(query.next()) {
				auto task = new TrainTask(query.record().value(QStringLiteral("Name")).toString(),
										  TrainTask::AgilityTask,
										  query.record().value(QStringLiteral("Count")).toInt(),
										  0);
				resList.append(QSharedPointer<TrainTask>(task));
			}
			emit tasksLoaded(TrainTask::AgilityTask, resList);
		} else
			emit managerError(query.lastError().text(), true);
	});
}

void TrainDataManager::completeTasks(const QDateTime &date, TrainDataManager::TaskResult result)
{

}
