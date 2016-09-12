#ifndef REMINDERMANAGER_H
#define REMINDERMANAGER_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QMap>
#include "timering.h"
#include "jsonstore.h"

class ReminderManager : public QObject
{
	Q_OBJECT

public:
	explicit ReminderManager(QObject *parent = nullptr);

	void addReminder(const QTime &time, bool intense);
	void removeReminder(const QTime &time);

	static QByteArray exportReminders();

signals:
	void reminderTriggered(bool intense);

private slots:
	void timerTriggered();
	void startNext(const QTime &time);
	void restart();
	void doStore();

private:
	JsonStore store;
	TimeRing timeRing;
	QHash<QTime, bool> reminders;

	QTimer *remTimer;
};

#endif // REMINDERMANAGER_H
