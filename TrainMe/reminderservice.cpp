#include "reminderservice.h"
#include <QCoreApplication>
#ifdef Q_OS_WIN
#include "winreminderservice.h"
#elif defined(Q_OS_ANDROID)
#include "droidreminderservice.h"
#endif

ReminderService* ReminderService::_instance(nullptr);

ReminderService::ReminderService(QObject *parent) :
	QObject(parent)
{}

ReminderService *ReminderService::instance()
{
	if(!_instance) {
#ifdef Q_OS_WIN
		_instance = new WinReminderService(qApp);
#elif defined(Q_OS_ANDROID)
		_instance = new DroidReminderService(qApp);
#endif
	}
	return _instance;
}
