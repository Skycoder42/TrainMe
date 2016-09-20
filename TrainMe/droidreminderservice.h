#ifndef DROIDREMINDERSERVICE_H
#define DROIDREMINDERSERVICE_H

#include <QObject>
#include <QtAndroidExtras>
#include "reminderservice.h"

class DroidReminderService : public ReminderService
{
	Q_OBJECT

public:
	explicit DroidReminderService(QObject *parent = nullptr);

public slots:
	void loadState() override;
	void setRemindersActive(bool active) override;
	void addReminder(const QTime &time, bool intense) override;
	void removeReminder(const QTime &time) override;
	void skipReminder(const QDate &skipDate) override;
	void setPermanent(bool permanent) override;
	void setGifTag(QString gifTag) override;

private:
	QAndroidJniObject reminderController;
};

#endif // DROIDREMINDERSERVICE_H
