#include "traincontrol.h"

TrainControl::TrainControl(QObject *parent) :
	ViewControl(parent),
	strengthModel(new TrainModel(TrainTask::StrengthTask, this)),
	agilityModel(new TrainModel(TrainTask::AgilityTask, this)),
	trainingAllowed(false)
{
	connect(this->manager, &TrainDataManager::tasksLoaded,
			this->strengthModel, &TrainModel::resetData,
			Qt::QueuedConnection);
	connect(this->manager, &TrainDataManager::tasksLoaded,
			this->agilityModel, &TrainModel::resetData,
			Qt::QueuedConnection);
	connect(this->manager, &TrainDataManager::traingAllowedLoaded,
			this, &TrainControl::updateAllowed,
			Qt::QueuedConnection);
	connect(this->strengthModel, &TrainModel::allDoneChanged,
			this, &TrainControl::allDoneChanged);
	connect(this->agilityModel, &TrainModel::allDoneChanged,
			this, &TrainControl::allDoneChanged);
}

bool TrainControl::allDone() const
{
	return this->strengthModel->allDone() &&
			this->agilityModel->allDone();
}

void TrainControl::initialize()
{
	this->manager->loadTrainingAllowed();
	this->manager->loadAllTasks();
}

void TrainControl::completeTraining()
{
	if(this->allDone())
		this->manager->completeTasks(QDate::currentDate(), TrainDataManager::Done);
	else
		this->manager->completeTasks(QDate::currentDate(), TrainDataManager::Fail);
	this->updateAllowed(false);
}

void TrainControl::updateAllowed(bool allowed)
{
	this->trainingAllowed = allowed;
	emit trainingAllowedChanged(allowed);
}
