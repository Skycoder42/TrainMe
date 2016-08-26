#ifndef WEEKCONFIGMODEL_H
#define WEEKCONFIGMODEL_H

#include <QAbstractListModel>
#include "traindatamanager.h"

class WeekConfigModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum Roles {
		Weekday = Qt::UserRole + 1,
		Increment,
		AddTask
	};

	explicit WeekConfigModel(QObject *parent = nullptr);

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QHash<int, QByteArray> roleNames() const override;

public slots:
	void resetModel(const TrainDataManager::Weekonfig &weekConfig);

private:
	QHash<Qt::DayOfWeek, TrainDataManager::DayConfig> dayData;
};

#endif // WEEKCONFIGMODEL_H
