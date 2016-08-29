#ifndef TRAINDATAMANAGER_H
#define TRAINDATAMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include <QDateTime>
#include <QSharedPointer>
#include <QThreadPool>
#include <functional>
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

	//trainControl
	void loadTrainingAllowed();
	void loadAllTasks();

	//resultControl
	void loadTaskResults(bool fillDates);

	//weekConfigControl
	void loadWeekConfig();
	void updateWeekConfigIncrement(Qt::DayOfWeek dayOfWeek, int increment);
	void updateWeekConfigAddTask(Qt::DayOfWeek dayOfWeek, bool addTask);
	void updatePenaltyFactor(double penaltyFactor);
	void updateMaxFreeDays(int maxFreeDays);
	void updateAgilityPenalties(bool agilityPenalties);
	void restoreWeekDefaults();

	//createTask
	void loadFreeTasks();
	void addTask(const QSharedPointer<TrainTask> &task);

	//all
	void completeTasks(const QDate &date, TaskResult result);

signals:
	//special
	void managerReady(int startIndex);
	void managerMessage(const QString &title, const QString &message, bool isFatalError);
	void operationStarted();
	void operationCompleted();

	//trainControl
	void traingAllowedLoaded(bool allowed);
	void tasksLoaded(TrainTask::TaskType type, const QList<QSharedPointer<TrainTask>> &tasks);

	//resultControl
	void taskResultsLoaded(const QList<TrainDataManager::ResultInfo> &resultList);

	//weekConfigControl
	void weekConfigLoaded(const TrainDataManager::Weekonfig &weekConfig);
	void configExtrasLoaded(double penaltyFactor, int maxFreeDays, bool agilityPenalties);

	//createTask
	void freeTasksLoaded(int freeTasks, bool isAgility);

	//all
	void resetDone();

private:
	QSqlDatabase database;
	mutable QThreadPool *dbThread;

	void executeAsync(const std::function<void()> &fn);

	void recalcScores(const QDate &date);
	void addPenalty(int amount = 1);
	int addFree(const QDate &date);
	void resetPenalty(bool reduceOnly = false);

	int loadMissingTasks(bool *isAgilityTask = nullptr);
	bool testHasMissingTasks();
	bool testHasMissingDates();
};

Q_DECLARE_METATYPE(QList<TrainDataManager::ResultInfo>)
Q_DECLARE_METATYPE(TrainDataManager::Weekonfig)

#endif // TRAINDATAMANAGER_H
