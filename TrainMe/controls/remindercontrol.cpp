#include "remindercontrol.h"
#include "app.h"

ReminderControl::ReminderControl(QObject *parent) :
	ViewControl(parent),
	reminderService(ReminderService::createInstance(this)),
	active(false),
	permaShown(false),
	searchTag()
{}

bool ReminderControl::supportsReminders() const
{
	return this->reminderService;
}

bool ReminderControl::remindersActive() const
{
	return this->active;
}

bool ReminderControl::permanent() const
{
	return this->permaShown;
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
	if (this->permaShown == permanent)
		return;

	this->permaShown = permanent;
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

	} else {
		emit App::instance()->errorMessage(tr("Reminders not supported!"),
										   tr("The current platform does not support reminders. You cannot use "
											  "reminders on this device."),
										   false);
	}
}
