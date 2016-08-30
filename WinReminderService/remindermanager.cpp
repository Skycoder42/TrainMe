#include "remindermanager.h"

ReminderManager::ReminderManager(QObject *parent) :
	QObject(parent),
	timeRing(),
	reminders(),
	remTimer(new QTimer(this))
{
	this->remTimer->setSingleShot(true);
	connect(this->remTimer, &QTimer::timeout,
			this, &ReminderManager::timerTriggered);
}

void ReminderManager::addReminder(const QTime &time, bool intense)
{
	this->reminders.insert(time, intense);
	this->timeRing.insertTime(time);
	this->restart();
}

void ReminderManager::removeReminder(const QTime &time)
{
	this->reminders.remove(time);
	if(this->timeRing.removeTime(time))
		this->restart();
}

QHash<QTime, bool> ReminderManager::listReminders()
{
	return this->reminders;
}

void ReminderManager::timerTriggered()
{
	auto intense = this->reminders.value(this->timeRing.current());
	this->startNext(this->timeRing.next());
	emit reminderTriggered(intense);
}

void ReminderManager::startNext(const QTime &time)
{
	if(time.isValid()) {
		auto nextMSecs = QTime::currentTime().msecsTo(time);
		if(nextMSecs < 0)
			nextMSecs += 86400000;
		this->remTimer->start(nextMSecs);
	}
}

void ReminderManager::restart()
{
	this->remTimer->stop();
	this->timeRing.resetCurrent();
	this->startNext(this->timeRing.current());
}
