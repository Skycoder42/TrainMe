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
		Free = 5,

		Unknown = -1
	};
	Q_ENUM(TaskResult)

	typedef QPair<QDate, TrainDataManager::TaskResult> ResultInfo;
	typedef QPair<int, bool> DayConfig;
	typedef QHash<Qt::DayOfWeek, TrainDataManager::DayConfig> Weekonfig;

	explicit TrainDataManager(QObject *parent = nullptr);
	~TrainDataManager();

	Q_INVOKABLE static QString trResult(int taskResult);

public slots:
	void initManager();

	void loadTrainingAllowed();
	void loadStrengthTasks();
	void loadAgilityTasks();

	void loadTaskResults(bool fillDates);

	void loadWeekConfig();
	void updateWeekConfigIncrement(Qt::DayOfWeek dayOfWeek, int increment);
	void updateWeekConfigAddTask(Qt::DayOfWeek dayOfWeek, bool addTask);

	void completeTasks(const QDate &date, TaskResult result);

signals:
	void managerReady(int startIndex);
	void managerError(QString error, bool isFatal);

	void traingAllowedLoaded(bool allowed);
	void tasksLoaded(TrainTask::TaskType type, const QList<QSharedPointer<TrainTask>> &tasks);

	void taskResultsLoaded(const QList<TrainDataManager::ResultInfo> &resultList);

	void weekConfigLoaded(const TrainDataManager::Weekonfig &weekConfig);

	void operationStarted();
	void operationCompleted();

private:
	QSqlDatabase database;
	mutable QThreadPool *dbThread;

	void recalcScores(const QDate &date);
	void addPenalty();

	bool testHasMissingDates();
};

Q_DECLARE_METATYPE(QList<TrainDataManager::ResultInfo>)
Q_DECLARE_METATYPE(TrainDataManager::Weekonfig)

#endif // TRAINDATAMANAGER_H
