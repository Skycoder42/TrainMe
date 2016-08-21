#ifndef TRAINDATAMANAGER_H
#define TRAINDATAMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include <QDateTime>
#include <QSharedPointer>
#include <QThreadPool>
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
		Fail = 4,
		Free = 5
	};

	explicit TrainDataManager(QObject *parent = nullptr, const char *initSlot = nullptr);
	~TrainDataManager();

public slots:
	void loadTrainingAllowed();
	void loadStrengthTasks();
	void loadAgilityTasks();

	void completeTasks(const QDate &date, TaskResult result);

signals:
	void managerReady(QString error);
	void managerError(QString error, bool isFatal);

	void traingAllowedLoaded(bool allowed);
	void tasksLoaded(TrainTask::TaskType type, const QList<QSharedPointer<TrainTask>> &tasks);

	void operationStarted();
	void operationCompleted();

private:
	QSqlDatabase database;
	mutable QThreadPool *dbThread;

	void recalcScores(const QDate &date);
	void addPenalty();
};

#endif // TRAINDATAMANAGER_H
