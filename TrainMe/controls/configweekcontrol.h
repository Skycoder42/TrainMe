#ifndef CONFIGWEEKCONTROL_H
#define CONFIGWEEKCONTROL_H

#include <QObject>
#include "viewcontrol.h"
#include "weekconfigmodel.h"

class ConfigWeekControl : public ViewControl
{
	Q_OBJECT

	Q_PROPERTY(QObject* configModel MEMBER configModel CONSTANT)
	Q_PROPERTY(double penaltyFactor MEMBER penaltyFactor NOTIFY penaltyFactorChanged)
	Q_PROPERTY(int maxFreeDays MEMBER maxFreeDays NOTIFY maxFreeDaysChanged)
	Q_PROPERTY(bool agilityPenalties MEMBER agilityPenalties NOTIFY agilityPenaltiesChanged)

public:
	explicit ConfigWeekControl(QObject *parent = nullptr);

signals:
	void penaltyFactorChanged(double penaltyFactor);
	void maxFreeDaysChanged(int maxFreeDays);
	void agilityPenaltiesChanged(bool agilityPenalties);

protected:
	void doInit() override;
	void actionTriggered(int id) override;

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
