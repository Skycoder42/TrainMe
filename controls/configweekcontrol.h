#ifndef CONFIGWEEKCONTROL_H
#define CONFIGWEEKCONTROL_H

#include <QObject>
#include "weekconfigmodel.h"

class ConfigWeekControl : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QObject* configModel MEMBER configModel CONSTANT)
	Q_PROPERTY(double penaltyFactor MEMBER penaltyFactor NOTIFY penaltyFactorChanged)
	Q_PROPERTY(int maxFreeDays MEMBER maxFreeDays NOTIFY maxFreeDaysChanged)

public:
	explicit ConfigWeekControl(QObject *parent = 0);

public slots:
	void initialize();

signals:
	void penaltyFactorChanged(double penaltyFactor);
	void maxFreeDaysChanged(int maxFreeDays);

private slots:
	void updateExtraData(double penaltyFactor, int maxFreeDays);

private:
	WeekConfigModel *configModel;
	double penaltyFactor;
	int maxFreeDays;
};

#endif // CONFIGWEEKCONTROL_H
