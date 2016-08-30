#ifndef TRAINCONTROL_H
#define TRAINCONTROL_H

#include <QObject>
#include "viewcontrol.h"
#include "trainmodel.h"

class TrainControl : public ViewControl
{
	Q_OBJECT

	Q_PROPERTY(TrainModel* strengthModel MEMBER strengthModel CONSTANT)
	Q_PROPERTY(TrainModel* agilityModel MEMBER agilityModel CONSTANT)
	Q_PROPERTY(bool allDone READ allDone NOTIFY allDoneChanged)
	Q_PROPERTY(bool trainingAllowed MEMBER trainingAllowed NOTIFY trainingAllowedChanged)

public:
	explicit TrainControl(QObject *parent = nullptr);

	bool allDone() const;

public slots:
	void completeTraining();

signals:
	void allDoneChanged();	
	void trainingAllowedChanged(bool trainingAllowed);

protected:
	void doInit() override;

private slots:
	void updateAllowed(bool allowed);

private:
	TrainModel *strengthModel;
	TrainModel *agilityModel;
	bool trainingAllowed;
};

#endif // TRAINCONTROL_H
