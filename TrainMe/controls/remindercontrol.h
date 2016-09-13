#ifndef REMINDERCONTROL_H
#define REMINDERCONTROL_H

#include <QObject>
#include "viewcontrol.h"
#include "reminderservice.h"

class ReminderControl : public ViewControl
{
	Q_OBJECT

	Q_PROPERTY(bool supportsReminders READ supportsReminders CONSTANT)
	Q_PROPERTY(bool remindersActive READ areRemindersActive WRITE setRemindersActive NOTIFY remindersActiveChanged)
	Q_PROPERTY(bool permanent READ isPermanent WRITE setPermanent NOTIFY permanentChanged)
	Q_PROPERTY(QString gifTag READ gifTag WRITE setGifTag NOTIFY gifTagChanged)

public:
	explicit ReminderControl(QObject *parent = nullptr);

	bool supportsReminders() const;
	bool areRemindersActive() const;
	bool isPermanent() const;
	QString gifTag() const;

public slots:
	void setRemindersActive(bool remindersActive);
	void setPermanent(bool permanent);
	void setGifTag(QString gifTag);

signals:
	void remindersActiveChanged(bool remindersActive);
	void permanentChanged(bool permanent);
	void gifTagChanged(QString gifTag);

protected:
	void doInit() override;

private slots:
	void stateLoaded(bool active,
					 bool permanent,
					 const QString &gifTag,
					 const QHash<QTime, bool> &reminders);

private:
	ReminderService *reminderService;

	bool active;
	bool permanent;
	QString searchTag;
};

#endif // REMINDERCONTROL_H
