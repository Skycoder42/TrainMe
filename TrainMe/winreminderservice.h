#ifndef WINREMINDERSERVICE_H
#define WINREMINDERSERVICE_H

#include <QObject>
#include "reminderservice.h"

class WinReminderService : public ReminderService
{
	Q_OBJECT

public:
	explicit WinReminderService(QObject *parent = nullptr);

public slots:
	// ReminderService interface
	void loadState() override;
	void setRemindersActive(bool active) override;
	void addReminder(const QTime &time, bool intense) override;
	void removeReminder(const QTime &time) override;
	void skipReminder(const QDate &skipDate) override;
	void setPermanent(bool permanent) override;
	void setGifTag(QString gifTag) override;

private:
	QString remPath() const;
};

#endif // WINREMINDERSERVICE_H
