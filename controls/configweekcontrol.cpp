#include "configweekcontrol.h"

ConfigWeekControl::ConfigWeekControl(QObject *parent) :
	ViewControl(parent),
	configModel(new WeekConfigModel(this)),
	penaltyFactor(0.0),
	maxFreeDays(0),
	agilityPenalties(false)
{
	connect(this->manager, &TrainDataManager::weekConfigLoaded,
			this->configModel, &WeekConfigModel::resetModel,
			Qt::QueuedConnection);
	connect(this->manager, &TrainDataManager::configExtrasLoaded,
			this, &ConfigWeekControl::updateExtraData,
			Qt::QueuedConnection);
	connect(this, &ConfigWeekControl::penaltyFactorChanged,
			this->manager, &TrainDataManager::updatePenaltyFactor);
	connect(this, &ConfigWeekControl::maxFreeDaysChanged,
			this->manager, &TrainDataManager::updateMaxFreeDays);
	connect(this, &ConfigWeekControl::agilityPenaltiesChanged,
			this->manager, &TrainDataManager::updateAgilityPenalties);

	this->addAction(0, tr("Restore Defaults"));
}

void ConfigWeekControl::initialize()
{
	this->manager->loadWeekConfig();
}

void ConfigWeekControl::actionTriggered(int id)
{
	switch(id) {
	case 0:
		this->manager->restoreWeekDefaults();
	}
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
