#include "configweekcontrol.h"
#include "app.h"

ConfigWeekControl::ConfigWeekControl(QObject *parent) :
	QObject(parent),
	configModel(new WeekConfigModel(this))
{
	connect(App::instance()->trainManager(), &TrainDataManager::weekConfigLoaded,
			this->configModel, &WeekConfigModel::resetModel,
			Qt::QueuedConnection);
}

void ConfigWeekControl::initialize()
{
	App::instance()->trainManager()->loadWeekConfig();
}
