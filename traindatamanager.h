#ifndef TRAINDATAMANAGER_H
#define TRAINDATAMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include <QSharedPointer>
#include <QReadWriteLock>
#include "traintask.h"

class TrainDataManager : public QObject
{
	Q_OBJECT
public:
	enum TaskResult {
		Done = 0,
		Skip = 1,
		Sick = 2,
		Gain = 3,
		Fail = 4
	};

	explicit TrainDataManager(QObject *parent = nullptr, const char *initSlot = nullptr);
	~TrainDataManager();

public slots:
	void loadStrengthTasks();
	void loadAgilityTasks();

signals:
	void managerReady(QString error);
	void tasksLoaded(TrainTask::TaskType type, const QList<QSharedPointer<TrainTask>> &tasks);
	void managerError(QString error, bool isFatal);

private:
	QSqlDatabase database;
	mutable QReadWriteLock dbLock;
};

#endif // TRAINDATAMANAGER_H
