#include "remindercontrol.h"

ReminderControl::ReminderControl(QObject *parent) :
	ViewControl(parent)
{

}

bool ReminderControl::permanent() const
{
	return this->permaShown;
}

QString ReminderControl::gifTag() const
{
	return this->searchTag;
}

void ReminderControl::setPermanent(bool permanent)
{
	if (this->permaShown == permanent)
		return;

	this->permaShown = permanent;
	emit permanentChanged(permanent);
}

void ReminderControl::setGifTag(QString gifTag)
{
	if (this->searchTag == gifTag)
		return;

	this->searchTag = gifTag;
	emit gifTagChanged(gifTag);
}

void ReminderControl::doInit()
{
}
