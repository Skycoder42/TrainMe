#include "configweekcontrol.h"
#include "app.h"

ConfigWeekControl::ConfigWeekControl(QObject *parent) :
	QObject(parent),
	configModel(new WeekConfigModel(this)),
	penaltyFactor(0.0),
	maxFreeDays(0)
{
	connect(App::instance()->trainManager(), &TrainDataManager::weekConfigLoaded,
			this->configModel, &WeekConfigModel::resetModel,
			Qt::QueuedConnection);
	connect(App::instance()->trainManager(), &TrainDataManager::configExtrasLoaded,
			this, &ConfigWeekControl::updateExtraData,
			Qt::QueuedConnection);
	connect(this, &ConfigWeekControl::penaltyFactorChanged,
			App::instance()->trainManager(), &TrainDataManager::updatePenaltyFactor);
	connect(this, &ConfigWeekControl::maxFreeDaysChanged,
			App::instance()->trainManager(), &TrainDataManager::updateMaxFreeDays);
}

void ConfigWeekControl::initialize()
{
	App::instance()->trainManager()->loadWeekConfig();
}

void ConfigWeekControl::updateExtraData(double penaltyFactor, int maxFreeDays)
{
	this->penaltyFactor = penaltyFactor;
	this->maxFreeDays = maxFreeDays;
	emit penaltyFactorChanged(penaltyFactor);
	emit maxFreeDaysChanged(maxFreeDays);
}
