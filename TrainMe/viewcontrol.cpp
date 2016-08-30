#include "viewcontrol.h"
#include "app.h"

ViewControl::ViewControl(QObject *parent) :
	QObject(parent),
	manager(App::instance()->trainManager()),
	isActive(false)
{
	connect(this->manager, &TrainDataManager::resetDone,
			this, &ViewControl::completeReset,
			Qt::QueuedConnection);
}

QStringList ViewControl::menuActions() const
{
	return this->actions;
}

void ViewControl::initialize()
{
	if(!this->isActive) {
		this->doInit();
		this->isActive = true;
	}
}

void ViewControl::finalize()
{
	this->isActive = false;
}

void ViewControl::triggerAction(int index)
{
	this->actionTriggered(this->actionIds.value(index, -1));
}

void ViewControl::addAction(int id, const QString &action)
{
	this->actionIds.insert(this->actions.size(), id);
	this->actions.append(action);
	emit menuActionsChanged(this->actions);
}

void ViewControl::actionTriggered(int id)
{
	Q_UNUSED(id);
}

void ViewControl::completeReset()
{
	if(this->isActive)
		this->doInit();
}
