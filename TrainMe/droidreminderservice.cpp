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
								  Q_ARG(bool, this->reminderController.callMethod<jboolean>("isActive")),
								  Q_ARG(bool, this->reminderController.callMethod<jboolean>("isAlwaysVisible")),
								  Q_ARG(QString, this->reminderController.callObjectMethod<jstring>("gifTag").toString()),
								  Q_ARG(ReminderService::ReminderHash, ReminderHash()));
	});
}

void DroidReminderService::setRemindersActive(bool active)
{
	QtAndroid::runOnAndroidThread([=](){
		this->reminderController.callMethod<void>("activateReminders",
												  "(Z)V",
												  (jboolean)active);
	});
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
	QtAndroid::runOnAndroidThread([=](){
		this->reminderController.callMethod<void>("showAlways",
												  "(Z)V",
												  (jboolean)permanent);
	});
}

void DroidReminderService::setGifTag(QString gifTag)
{
}
