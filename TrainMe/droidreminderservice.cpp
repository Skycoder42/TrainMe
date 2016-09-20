#include "droidreminderservice.h"

DroidReminderService::DroidReminderService(QObject *parent) :
	ReminderService(parent),
	reminderController()
{
	QtAndroid::runOnAndroidThreadSync([this](){
		auto activity = QtAndroid::androidActivity();
		this->reminderController = activity.callObjectMethod("createController",
															 "()Lcom/Skycoder42/TrainMe/ReminderController;");
	});
}

void DroidReminderService::loadState()
{
	QtAndroid::runOnAndroidThread([this](){
		QMetaObject::invokeMethod(this, "stateLoaded", Qt::QueuedConnection,
								  Q_ARG(bool, false),
								  Q_ARG(bool, false),
								  Q_ARG(QString, QString()),
								  Q_ARG(ReminderService::ReminderHash, ReminderHash()));
	});
}

void DroidReminderService::setRemindersActive(bool active)
{
}

void DroidReminderService::addReminder(const QTime &time, bool intense)
{
}

void DroidReminderService::removeReminder(const QTime &time)
{
}

void DroidReminderService::skipReminder(const QDate &skipDate)
{
}

void DroidReminderService::setPermanent(bool permanent)
{
}

void DroidReminderService::setGifTag(QString gifTag)
{
}
