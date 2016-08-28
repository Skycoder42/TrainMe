#include "configweekcontrol.h"
#include "app.h"

ConfigWeekControl::ConfigWeekControl(QObject *parent) :
	QObject(parent),
	configModel(new WeekConfigModel(this)),
	penaltyFactor(0.0),
	maxFreeDays(0),
	agilityPenalties(false)
{
	connect(App::instance()->trainManager(), &TrainDataManager::weekConfigLoaded,
			this->configModel, &WeekConfigModel::resetModel,
			Qt::QueuedConnection);
	connect(App::instance()->trainManager(), &TrainDataManager::configExtrasLoaded,
			this, &ConfigWeekControl::updateExtraData,
			Qt::QueuedConnection);
	connect(App::instance()->trainManager(), &TrainDataManager::resetDone,
			this, &ConfigWeekControl::initialize,
			Qt::QueuedConnection);//TODO BASE CLASS!!!
	connect(this, &ConfigWeekControl::penaltyFactorChanged,
			App::instance()->trainManager(), &TrainDataManager::updatePenaltyFactor);
	connect(this, &ConfigWeekControl::maxFreeDaysChanged,
			App::instance()->trainManager(), &TrainDataManager::updateMaxFreeDays);
	connect(this, &ConfigWeekControl::agilityPenaltiesChanged,
			App::instance()->trainManager(), &TrainDataManager::updateAgilityPenalties);
}

void ConfigWeekControl::initialize()
{
	App::instance()->trainManager()->loadWeekConfig();
}

void ConfigWeekControl::restoreDefaults()
{
	App::instance()->trainManager()->restoreWeekDefaults();
}

void ConfigWeekControl::updateExtraData(double penaltyFactor, int maxFreeDays, bool agilityPenalties)
{
	this->penaltyFactor = penaltyFactor;
	this->maxFreeDays = maxFreeDays;
	this->agilityPenalties = agilityPenalties;
	emit penaltyFactorChanged(penaltyFactor);
	emit maxFreeDaysChanged(maxFreeDays);
	emit agilityPenaltiesChanged(agilityPenalties);
}
