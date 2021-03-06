#ifndef REMINDERCONTROL_H
#define REMINDERCONTROL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include "qelementmodel.h"
#include "viewcontrol.h"
#include "reminderservice.h"

class ReminderControl : public ViewControl
{
	Q_OBJECT

	Q_PROPERTY(bool supportsReminders READ supportsReminders CONSTANT)
	Q_PROPERTY(bool remindersActive READ areRemindersActive WRITE setRemindersActive NOTIFY remindersActiveChanged)
	Q_PROPERTY(bool permanent READ isPermanent WRITE setPermanent NOTIFY permanentChanged)
	Q_PROPERTY(QString gifTag READ gifTag WRITE setGifTag NOTIFY gifTagChanged)
	Q_PROPERTY(QObject* reminderModel MEMBER sortProxy CONSTANT)

public:
	explicit ReminderControl(QObject *parent = nullptr);

	bool supportsReminders() const;
	bool areRemindersActive() const;
	bool isPermanent() const;
	QString gifTag() const;
	QObjectList reminders() const;

public slots:
	void createReminder(QTime time, bool intense);
	void removeReminder(int index);

	void setRemindersActive(bool remindersActive);
	void setPermanent(bool permanent);
	void setGifTag(QString gifTag);

signals:
	void remindersActiveChanged(bool remindersActive);
	void permanentChanged(bool permanent);
	void gifTagChanged(QString gifTag);

	void remindersChanged(QObjectList reminders);

protected:
	void doInit() override;

private slots:
	void stateLoaded(bool active,
					 bool permanent,
					 const QString &gifTag,
					 const ReminderService::ReminderHash &reminders);

private:
	ReminderService *reminderService;

	bool active;
	bool permanent;
	QString searchTag;
	QElementModel *reminderModel;
	QSortFilterProxyModel *sortProxy;
};

#endif // REMINDERCONTROL_H
