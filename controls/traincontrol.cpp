#include "traincontrol.h"
#include "app.h"

TrainControl::TrainControl(QObject *parent) :
	QObject(parent),
	strengthModel(new TrainModel(TrainTask::StrengthTask, this)),
	agilityModel(new TrainModel(TrainTask::AgilityTask, this))
{
	connect(App::instance()->trainManager(), &TrainDataManager::tasksLoaded,
			this->strengthModel, &TrainModel::resetData,
			Qt::QueuedConnection);
	connect(App::instance()->trainManager(), &TrainDataManager::tasksLoaded,
			this->agilityModel, &TrainModel::resetData,
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
	App::instance()->trainManager()->loadStrengthTasks();
	App::instance()->trainManager()->loadAgilityTasks();
}

void TrainControl::completeTraining()
{
	if(this->allDone())
		App::instance()->trainManager()->completeTasks(QDateTime::currentDateTime(), TrainDataManager::Done);
	else
		App::instance()->trainManager()->completeTasks(QDateTime::currentDateTime(), TrainDataManager::Fail);
}
