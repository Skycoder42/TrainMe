#include "remindermanager.h"
#include <QJsonObject>
#include <QJsonArray>

#define TIME_KEY QStringLiteral("time")
#define INTENSE_KEY QStringLiteral("intense")

ReminderManager::ReminderManager(QObject *parent) :
	QObject(parent),
	store(QStringLiteral("remindcache")),
	timeRing(),
	reminders(),
	remTimer(new QTimer(this))
{
	this->remTimer->setSingleShot(true);
	connect(this->remTimer, &QTimer::timeout,
			this, &ReminderManager::timerTriggered);

	//load reminders
	auto remArray = this->store.load().toArray();
	foreach(auto value, remArray) {
		auto remObj = value.toObject();
		auto time = QTime::fromString(remObj[TIME_KEY].toString(), Qt::ISODate);
		auto intense = remObj[INTENSE_KEY].toBool();
		this->reminders.insert(time, intense);
		this->timeRing.insertTime(time);
	}
	this->restart();
}

void ReminderManager::addReminder(const QTime &time, bool intense)
{
	this->reminders.insert(time, intense);
	this->timeRing.insertTime(time);
	this->restart();
	this->doStore();
}

void ReminderManager::removeReminder(const QTime &time)
{
	this->reminders.remove(time);
	if(this->timeRing.removeTime(time))
		this->restart();
	this->doStore();
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

void ReminderManager::doStore()
{
	QJsonArray remArray;
	for (auto it = this->reminders.constBegin(), end = this->reminders.constEnd(); it != end; ++it) {
		QJsonObject remObj;
		remObj[TIME_KEY] = it.key().toString(Qt::ISODate);
		remObj[INTENSE_KEY] = it.value();
		remArray.append(remObj);
	}
	this->store.save(remArray);
}
