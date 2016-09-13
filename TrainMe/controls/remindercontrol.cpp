#include "remindercontrol.h"
#include "app.h"

ReminderControl::ReminderControl(QObject *parent) :
	ViewControl(parent),
	reminderService(ReminderService::createInstance(this)),
	active(false),
	permanent(false),
	searchTag()
{
	connect(this->reminderService, &ReminderService::stateLoaded,
			this, &ReminderControl::stateLoaded);
}

bool ReminderControl::supportsReminders() const
{
	return this->reminderService;
}

bool ReminderControl::areRemindersActive() const
{
	return this->active;
}

bool ReminderControl::isPermanent() const
{
	return this->permanent;
}

QString ReminderControl::gifTag() const
{
	return this->searchTag;
}

void ReminderControl::setRemindersActive(bool remindersActive)
{
	if (this->active == remindersActive)
		return;

	this->active = remindersActive;
	this->reminderService->setRemindersActive(remindersActive);
	emit remindersActiveChanged(remindersActive);
}

void ReminderControl::setPermanent(bool permanent)
{
	if (this->permanent == permanent)
		return;

	this->permanent = permanent;
	this->reminderService->setPermanent(permanent);
	emit permanentChanged(permanent);
}

void ReminderControl::setGifTag(QString gifTag)
{
	if (this->searchTag == gifTag)
		return;

	this->searchTag = gifTag;
	this->reminderService->setGifTag(gifTag);
	emit gifTagChanged(gifTag);
}

void ReminderControl::doInit()
{
	if(this->reminderService) {
		emit this->manager->operationStarted();
		this->reminderService->loadState();
	} else {
		emit App::instance()->errorMessage(tr("Reminders not supported!"),
										   tr("The current platform does not support reminders. You cannot use "
											  "reminders on this device."),
										   false);
	}
}

void ReminderControl::stateLoaded(bool active, bool permanent, const QString &gifTag, const QHash<QTime, bool> &reminders)
{
	this->active = active;
	this->permanent = permanent;
	this->searchTag = gifTag;

	emit remindersActiveChanged(active);
	emit permanentChanged(permanent);
	emit gifTagChanged(gifTag);

	emit this->manager->operationCompleted();
}
