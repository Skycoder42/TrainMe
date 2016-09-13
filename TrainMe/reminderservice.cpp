#include "reminderservice.h"
#ifdef Q_OS_WIN
#include "winreminderservice.h"
#endif

ReminderService::ReminderService(QObject *parent) :
	QObject(parent)
{}

ReminderService *ReminderService::createInstance(QObject *parent)
{
#ifdef Q_OS_WIN
	return new WinReminderService(parent);
#else
	return nullptr;
#endif
}
