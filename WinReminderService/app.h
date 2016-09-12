#ifndef APP_H
#define APP_H

#include <QApplication>
#include <qsingleinstance.h>
#include <qpropertysettings.h>
#include "remindermanager.h"
#include "notifier.h"
#include "skipmanager.h"

class App : public QApplication
{
	Q_OBJECT
public:
	enum CommandTypes : char {
		Add = 'A',
		Remove = 'R',
		Skip = 'S',
		Permanent = 'P',
		Giftag = 'G',
		Quit = 'Q'
	};

	explicit App(int argc, char *argv[]);
	~App();

	void testTimeRing();

	bool testStandardArgs();
	int exec();

private slots:
	void handleCommand(const QStringList &args);

private:
	QSingleInstance *singleInstance;

	QPropertySettings *settings;
	SkipManager *skipper;
	ReminderManager *reminder;
	Notifier *notifier;

	int startup();
	void printState();
};

#endif // APP_H
