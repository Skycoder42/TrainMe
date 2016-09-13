#ifndef REMINDERSERVICE_H
#define REMINDERSERVICE_H

#include <QTime>
#include <QHash>
#include <QObject>

class ReminderService : public QObject
{	
	Q_OBJECT

public:
	ReminderService(QObject *parent = nullptr);

	static ReminderService* createInstance(QObject *parent = nullptr);

public slots:
	virtual void loadState() = 0;

	virtual void setRemindersActive(bool active) = 0;

	virtual void addReminder(const QTime &time, bool intense) = 0;
	virtual void removeReminder(const QTime &time) = 0;
	virtual void skipReminder(const QDate &skipDate) = 0;

	virtual void setPermanent(bool permanent) = 0;
	virtual void setGifTag(QString gifTag) = 0;

signals:
	void stateLoaded(bool active,
					 bool permanent,
					 const QString &gifTag,
					 const QHash<QTime, bool> &reminders);
};

#endif // REMINDERSERVICE_H
