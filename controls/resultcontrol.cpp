#include "resultcontrol.h"

ResultControl::ResultControl(QObject *parent) :
	ViewControl(parent),
	taskResultList()
{
	connect(this->manager, &TrainDataManager::taskResultsLoaded,
			this, &ResultControl::taskResultsLoaded,
			Qt::QueuedConnection);
}

QStringList ResultControl::resultList() const
{
	QStringList resList;
	foreach(auto item, this->taskResultList) {
		resList.append(tr("%1:  <b>%2</b>")
					   .arg(item.first.toString(Qt::DefaultLocaleShortDate))
					   .arg(TrainDataManager::trResult(item.second)));
	}
	return resList;
}

bool ResultControl::isEditable(int index) const
{
	return this->taskResultList[index].second == TrainDataManager::Unknown;
}

int ResultControl::firstDirtyIndex() const
{
	for(int i = 0, total = this->taskResultList.size(); i < total; ++i) {
		if(this->taskResultList[i].second == TrainDataManager::Unknown)
			return i;
	}
	return -1;
}

void ResultControl::initialize()
{
	this->manager->loadTaskResults(true);
}

void ResultControl::updateResult(int index, int result)
{
	auto &taskInfo = this->taskResultList[index];
	taskInfo.second = (TrainDataManager::TaskResult)result;
	this->manager->completeTasks(taskInfo.first, taskInfo.second);
	emit resultListChanged();
}

void ResultControl::taskResultsLoaded(const QList<TrainDataManager::ResultInfo> &resultList)
{
	this->taskResultList = resultList;
	emit resultListChanged();
}
