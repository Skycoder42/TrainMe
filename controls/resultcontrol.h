#ifndef RESULTCONTROL_H
#define RESULTCONTROL_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include "viewcontrol.h"
#include "traindatamanager.h"

class ResultControl : public ViewControl
{
	Q_OBJECT

	Q_PROPERTY(QStringList resultList READ resultList NOTIFY resultListChanged)

public:
	explicit ResultControl(QObject *parent = nullptr);

	QStringList resultList() const;

	Q_INVOKABLE bool isEditable(int index) const;
	Q_INVOKABLE int firstDirtyIndex() const;

public slots:
	void updateResult(int index, int result);
	void createResult(const QDateTime &date, int result);

signals:
	void resultListChanged();
	void createNewEntry();

protected:
	void doInit() override;
	void actionTriggered(int id) override;

private slots:
	void taskResultsLoaded(const QList<TrainDataManager::ResultInfo> &resultList);

private:
	QList<TrainDataManager::ResultInfo> taskResultList;
};

#endif // RESULTCONTROL_H
