#ifndef TRAINMODEL_H
#define TRAINMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QSharedPointer>
#include "traintask.h"

class TrainModel : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
	Q_PROPERTY(bool allDone READ allDone NOTIFY allDoneChanged)

public:
	enum Roles {
		NameRole = Qt::UserRole + 1,
		RepetitionsRole,
		CheckedRole
	};

	explicit TrainModel(TrainTask::TaskType type, QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QHash<int, QByteArray> roleNames() const override;

	bool allDone() const;

public slots:
	void resetData(TrainTask::TaskType type, const QList<QSharedPointer<TrainTask>> &tasks);

signals:
	void rowCountChanged();	
	void allDoneChanged();

private:
	TrainTask::TaskType type;
	QList<QSharedPointer<TrainTask>> rootList;
	int m_rowCount;
	bool m_allDone;
};

#endif // TRAINMODEL_H
