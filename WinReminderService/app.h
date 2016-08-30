#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include "remindermanager.h"

class App : public QCoreApplication
{
	Q_OBJECT
public:
	explicit App(int argc, char *argv[]);

	void testTimeRing();
private:
	ReminderManager *reminderManager;
};

#endif // APP_H
