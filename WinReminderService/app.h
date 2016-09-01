#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <qsingleinstance.h>
#include "remindermanager.h"

class App : public QCoreApplication
{
	Q_OBJECT
public:
	enum CommandTypes : char {
		Add = 'A',
		Remove = 'R',
		Skip = 'S'
	};

	explicit App(int argc, char *argv[]);

	void testTimeRing();

	bool testStandardArgs();
	int exec();

private slots:
	void handleCommand(const QStringList &args);

private:
	QSingleInstance *singleInstance;

	ReminderManager *manager;

	int startup();
};

#endif // APP_H
