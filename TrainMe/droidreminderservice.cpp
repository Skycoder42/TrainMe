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
		ReminderHash reminderHash;

		QAndroidJniEnvironment jniEnv;
		QAndroidJniObject reminders = this->reminderController.callObjectMethod("getReminders",
																				"()[Lcom/Skycoder42/TrainMe/ReminderController$ReminderInfo;");
		auto length = jniEnv->GetArrayLength(reminders.object<jarray>());
		for(auto i = 0; i < length; i++) {
			QAndroidJniObject info(jniEnv->GetObjectArrayElement(reminders.object<jobjectArray>(), i));
			auto hours = info.getField<jint>("hours");
			auto minutes = info.getField<jint>("minutes");
			reminderHash.insert(QTime(hours, minutes),
								info.getField<jboolean>("intense"));
		}


		QMetaObject::invokeMethod(this, "stateLoaded", Qt::QueuedConnection,
								  Q_ARG(bool, this->reminderController.callMethod<jboolean>("isActive")),
								  Q_ARG(bool, this->reminderController.callMethod<jboolean>("isAlwaysVisible")),
								  Q_ARG(QString, this->reminderController.callObjectMethod<jstring>("gifTag").toString()),
								  Q_ARG(ReminderService::ReminderHash, reminderHash));
	});
}

void DroidReminderService::setRemindersActive(bool active)
{
	QtAndroid::runOnAndroidThread([=](){
		this->reminderController.callMethod<void>("setActive",
												  "(Z)V",
												  (jboolean)active);
	});
}

void DroidReminderService::addReminder(const QTime &time, bool intense)
{
	QtAndroid::runOnAndroidThread([=](){
		QAndroidJniObject reminder("com/Skycoder42/TrainMe/ReminderController$ReminderInfo");
		reminder.setField<jint>("hours", time.hour());
		reminder.setField<jint>("minutes", time.minute());
		reminder.setField<jboolean>("intense", intense);
		this->reminderController.callMethod<void>("addReminder",
												  "(Lcom/Skycoder42/TrainMe/ReminderController$ReminderInfo;)V",
												  reminder.object());
	});
}

void DroidReminderService::removeReminder(const QTime &time)
{
	QtAndroid::runOnAndroidThread([=](){
		QAndroidJniObject reminder("com/Skycoder42/TrainMe/ReminderController$ReminderInfo");
		reminder.setField<jint>("hours", time.hour());
		reminder.setField<jint>("minutes", time.minute());
		this->reminderController.callMethod<void>("removeReminder",
												  "(Lcom/Skycoder42/TrainMe/ReminderController$ReminderInfo;)V",
												  reminder.object());
	});
}

void DroidReminderService::skipReminder(const QDate &skipDate)
{
	QtAndroid::runOnAndroidThread([=](){
		QAndroidJniObject jDate("java/util/Date",
								"(III)V",
								skipDate.year() - 1900,
								skipDate.month() - 1,
								skipDate.day());
		this->reminderController.callMethod<void>("skipDate",
												  "(Ljava/util/Date;)V",
												  jDate.object());
	});
}

void DroidReminderService::setPermanent(bool permanent)
{
	QtAndroid::runOnAndroidThread([=](){
		this->reminderController.callMethod<void>("setAlwaysVisible",
												  "(Z)V",
												  (jboolean)permanent);
	});
}

void DroidReminderService::setGifTag(QString gifTag)
{
	QtAndroid::runOnAndroidThread([=](){
		this->reminderController.callMethod<void>("setGifTag",
												  "(Ljava/lang/String;)V",
												  QAndroidJniObject::fromString(gifTag).object<jstring>());
	});
}
