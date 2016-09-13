#ifndef REMINDERCONTROL_H
#define REMINDERCONTROL_H

#include <QObject>
#include "viewcontrol.h"

class ReminderControl : public ViewControl
{
	Q_OBJECT

	Q_PROPERTY(bool permanent READ permanent WRITE setPermanent NOTIFY permanentChanged)
	Q_PROPERTY(QString gifTag READ gifTag WRITE setGifTag NOTIFY gifTagChanged)

public:
	explicit ReminderControl(QObject *parent = nullptr);

	bool permanent() const;
	QString gifTag() const;

public slots:
	void setPermanent(bool permanent);
	void setGifTag(QString gifTag);

signals:
	void permanentChanged(bool permanent);
	void gifTagChanged(QString gifTag);

protected:
	void doInit() override;

private:
	bool permaShown;
	QString searchTag;
};

#endif // REMINDERCONTROL_H
