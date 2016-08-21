#ifndef TRAINCONTROL_H
#define TRAINCONTROL_H

#include <QObject>
#include "trainmodel.h"

class TrainControl : public QObject
{
	Q_OBJECT

	Q_PROPERTY(TrainModel* strengthModel MEMBER strengthModel CONSTANT)
	Q_PROPERTY(TrainModel* agilityModel MEMBER agilityModel CONSTANT)
	Q_PROPERTY(bool allDone READ allDone NOTIFY allDoneChanged)

public:
	explicit TrainControl(QObject *parent = nullptr);

	bool allDone() const;

public slots:
	void initialize();

	void completeTraining();

signals:
	void allDoneChanged();

private:
	TrainModel *strengthModel;
	TrainModel *agilityModel;
};

#endif // TRAINCONTROL_H
