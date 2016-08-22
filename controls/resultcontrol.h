#ifndef RESULTCONTROL_H
#define RESULTCONTROL_H

#include <QObject>
#include <QList>
#include "traindatamanager.h"

class ResultControl : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QStringList resultList READ resultList NOTIFY resultListChanged)

public:
	explicit ResultControl(QObject *parent = nullptr);

	QStringList resultList() const;

	Q_INVOKABLE bool isEditable(int index) const;
	Q_INVOKABLE int firstDirtyIndex() const;

public slots:
	void initialize();

	void updateResult(int index, int result);

signals:
	void resultListChanged();

private slots:
	void taskResultsLoaded(const QList<TrainDataManager::ResultInfo> &resultList);

private:
	QList<QPair<QDate, TrainDataManager::TaskResult>> taskResultList;
};

#endif // RESULTCONTROL_H
