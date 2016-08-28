#include "createtaskcontrol.h"
#include <QDebug>

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

void CreateTaskControl::createTask(const QString &name, int baseCount, double increment)
{
	qDebug() << name << baseCount << increment;
}

void CreateTaskControl::freeTasksLoaded(int freeTasks, bool isAgility)
{
	this->freeTasks = freeTasks;
	this->nextIsAgility = isAgility;
	emit freeTasksChanged();
	emit nextIsAgilityChanged();
}
