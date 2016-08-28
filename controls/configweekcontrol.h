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
	Q_PROPERTY(bool agilityPenalties MEMBER agilityPenalties NOTIFY agilityPenaltiesChanged)

public:
	explicit ConfigWeekControl(QObject *parent = 0);

public slots:
	void initialize();
	void restoreDefaults();

signals:
	void penaltyFactorChanged(double penaltyFactor);
	void maxFreeDaysChanged(int maxFreeDays);
	void agilityPenaltiesChanged(bool agilityPenalties);

private slots:
	void updateExtraData(double penaltyFactor,
						 int maxFreeDays,
						 bool agilityPenalties);

private:
	WeekConfigModel *configModel;
	double penaltyFactor;
	int maxFreeDays;
	bool agilityPenalties;
};

#endif // CONFIGWEEKCONTROL_H
