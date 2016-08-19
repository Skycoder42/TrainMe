#include "traintask.h"

TrainTask::TrainTask(const QString &name, TaskType type, int baseCount, int increment) :
	QObject(nullptr),
	trainName(name),
	type(type),
	baseCount(baseCount),
	increment(increment),
	done(false)
{}

QString TrainTask::name() const
{
	return this->trainName;
}

int TrainTask::trainCount() const
{
	return this->baseCount + this->increment;
}

bool TrainTask::isDone() const
{
	return this->done;
}

TrainTask::TaskType TrainTask::taskType() const
{
	return this->type;
}

bool TrainTask::setDone(bool done)
{
	if (this->done == done)
		return false;

	this->done = done;
	emit doneChanged(done);
	return true;
}
