#ifndef CONFIGWEEKCONTROL_H
#define CONFIGWEEKCONTROL_H

#include <QObject>
#include "weekconfigmodel.h"

class ConfigWeekControl : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QObject* configModel MEMBER configModel CONSTANT)

public:
	explicit ConfigWeekControl(QObject *parent = 0);

public slots:
	void initialize();

private:
	WeekConfigModel *configModel;
};

#endif // CONFIGWEEKCONTROL_H
