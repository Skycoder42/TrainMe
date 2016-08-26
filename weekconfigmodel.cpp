#include "weekconfigmodel.h"
#include <QDate>
#include "app.h"

WeekConfigModel::WeekConfigModel(QObject *parent) :
	QAbstractListModel(parent),
	dayData()
{
	for(int day = Qt::Monday; day <= Qt::Sunday; day++)
		this->dayData.insert((Qt::DayOfWeek)day, {0, false});
}

int WeekConfigModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return 7;
}

QVariant WeekConfigModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() ||
		index.column() != 0 ||
		index.row() < 0 ||
		index.row() >= 7)
		return QVariant();

	auto day = (Qt::DayOfWeek)(index.row() + 1);
	switch (role) {
	case Weekday:
		return QDate::longDayName(day, QDate::StandaloneFormat);
	case Increment:
		return this->dayData[day].first;
	case AddTask:
		return this->dayData[day].second;
	default:
		return QVariant();
	}
}

bool WeekConfigModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() ||
		index.column() != 0 ||
		index.row() < 0 ||
		index.row() >= 7)
		return false;

	auto day = (Qt::DayOfWeek)(index.row() + 1);
	switch (role) {
	case Increment:
		if(this->dayData[day].first != value.toInt()) {
			this->dayData[day].first = value.toInt();
			emit dataChanged(index, index, {role});
			App::instance()->trainManager()->updateWeekConfigIncrement(day, value.toInt());
			return true;
		}
		break;
	case AddTask:
		if(this->dayData[day].second != value.toBool()) {
			this->dayData[day].second = value.toBool();
			emit dataChanged(index, index, {role});
			App::instance()->trainManager()->updateWeekConfigAddTask(day, value.toBool());
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

Qt::ItemFlags WeekConfigModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;
	return Qt::ItemIsEditable |
		   Qt::ItemIsEnabled |
		   Qt::ItemIsSelectable |
		   Qt::ItemNeverHasChildren;
}


QHash<int, QByteArray> WeekConfigModel::roleNames() const
{
	return {
		{Weekday, "weekday"},
		{Increment, "increment"},
		{AddTask, "addTask"}
	};
}

void WeekConfigModel::resetModel(const TrainDataManager::Weekonfig &weekConfig)
{
	this->beginResetModel();
	this->dayData = weekConfig;
	this->endResetModel();
}
