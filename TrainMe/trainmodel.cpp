#include "trainmodel.h"

TrainModel::TrainModel(TrainTask::TaskType type, QObject *parent) :
	QAbstractListModel(parent),
	type(type),
	rootList()
{
	connect(this, &TrainModel::rowsInserted,
			this, &TrainModel::rowCountChanged);
	connect(this, &TrainModel::rowsAboutToBeRemoved,
			this, &TrainModel::rowCountChanged);
	connect(this, &TrainModel::layoutChanged,
			this, &TrainModel::rowCountChanged);
	connect(this, &TrainModel::modelReset,
			this, &TrainModel::rowCountChanged);
}

int TrainModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return this->rootList.size();
}

QVariant TrainModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid() &&
		index.column() != 0 &&
		index.row() < 0 &&
		index.row() >= this->rootList.size())
		return QVariant();

	switch (role) {
	case NameRole:
		return this->rootList[index.row()]->name();
	case RepetitionsRole:
		return this->rootList[index.row()]->trainCount();
	case CheckedRole:
		return this->rootList[index.row()]->isDone();
	default:
		return QVariant();
	}
}

bool TrainModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() &&
		index.column() != 0 &&
		index.row() < 0 &&
		index.row() >= this->rootList.size()&&
		role != CheckedRole)
		return false;

	if(this->rootList[index.row()]->setDone(value.toBool())) {
		emit dataChanged(index, index, {CheckedRole});
		emit allDoneChanged();
		return true;
	} else
		return false;
}

Qt::ItemFlags TrainModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;
	else
		return this->QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
}


QHash<int, QByteArray> TrainModel::roleNames() const
{
	QHash<int, QByteArray> hash;
	hash[NameRole] = "name";
	hash[RepetitionsRole] = "repetitions";
	hash[CheckedRole] = "checked";
	return hash;
}

bool TrainModel::allDone() const
{
	foreach (auto item, this->rootList) {
		if(!item->isDone())
			return false;
	}
	return true;
}

void TrainModel::resetData(TrainTask::TaskType type, const QList<QSharedPointer<TrainTask> > &tasks)
{
	if(type == this->type) {
		this->beginResetModel();
		this->rootList = tasks;
		this->endResetModel();
		emit allDoneChanged();
	}
}
