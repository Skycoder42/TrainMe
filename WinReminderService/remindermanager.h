#ifndef REMINDERMANAGER_H
#define REMINDERMANAGER_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QMap>
#include "timering.h"

class ReminderManager : public QObject
{
	Q_OBJECT

public:
	explicit ReminderManager(QObject *parent = nullptr);

	void addReminder(const QTime &time, bool intense);
	void removeReminder(const QTime &time);

	QHash<QTime, bool> listReminders();

signals:
	void reminderTriggered(bool intense);

private slots:
	void timerTriggered();
	void startNext(const QTime &time);
	void restart();

private:
	TimeRing timeRing;
	QHash<QTime, bool> reminders;

	QTimer *remTimer;
};

#endif // REMINDERMANAGER_H
