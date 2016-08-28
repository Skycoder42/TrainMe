#include "traintask.h"

TrainTask::TrainTask(const QString &name, TaskType type, int baseCount, double factor, int increment, double penalty) :
	QObject(nullptr),
	trainName(name),
	type(type),
	baseCount(baseCount),
	factor(factor),
	increment(increment),
	penalty(penalty),
	done(false)
{}

QString TrainTask::name() const
{
	return this->trainName;
}

int TrainTask::trainCount() const
{
	return this->baseCount +
			qRound(this->factor * (this->increment + this->penalty));
}

bool TrainTask::isDone() const
{
	return this->done;
}

TrainTask::TaskType TrainTask::taskType() const
{
	return this->type;
}

void TrainTask::elements(int &baseCount, double &factor)
{
	baseCount = this->baseCount;
	factor = this->factor;
}

bool TrainTask::setDone(bool done)
{
	if (this->done == done)
		return false;

	this->done = done;
	emit doneChanged(done);
	return true;
}
