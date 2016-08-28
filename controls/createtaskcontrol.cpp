#include "createtaskcontrol.h"
#include "traintask.h"

CreateTaskControl::CreateTaskControl(QObject *parent) :
	ViewControl(parent),
	freeTasks(0),
	nextIsAgility(false)
{
	connect(this->manager, &TrainDataManager::freeTasksLoaded,
			this, &CreateTaskControl::freeTasksLoaded,
			Qt::QueuedConnection);
}

void CreateTaskControl::initialize()
{
	this->manager->loadFreeTasks();
}

void CreateTaskControl::createTask(const QString &name, int baseCount, double factor)
{
	QSharedPointer<TrainTask> task;
	if(this->nextIsAgility)
		task.reset(new TrainTask(name, TrainTask::AgilityTask, baseCount));
	else
		task.reset(new TrainTask(name, TrainTask::StrengthTask, baseCount, factor));
	this->manager->addTask(task);
}

void CreateTaskControl::freeTasksLoaded(int freeTasks, bool isAgility)
{
	this->freeTasks = freeTasks;
	this->nextIsAgility = isAgility;
	emit freeTasksChanged();
	emit nextIsAgilityChanged();
}
