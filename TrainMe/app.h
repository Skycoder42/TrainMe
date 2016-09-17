#ifndef APP_H
#define APP_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qpropertysettings.h>
#include "traindatamanager.h"
#include "trainmodel.h"
#ifdef Q_OS_WIN
#include <qsingleinstance.h>
#endif

#include "controls/traincontrol.h"
#include "controls/resultcontrol.h"
#include "controls/configweekcontrol.h"
#include "controls/createtaskcontrol.h"
#include "controls/remindercontrol.h"
#include "controls/motivatecontrol.h"

class App : public QGuiApplication
{
	Q_OBJECT

	Q_PROPERTY(TrainDataManager *trainManager READ trainManager CONSTANT)
	Q_PROPERTY(bool useMainColor MEMBER useMainColor NOTIFY useMainColorChanged)

public:
	explicit App(int argc, char *argv[]);

	static App* instance();
	TrainDataManager *trainManager() const;

	bool startupOk();

	Q_INVOKABLE bool testStyle(const QString &styleName) const;

#ifdef Q_OS_WIN
	int exec();
#endif

signals:
	void startupCompleted(int startIndex);
	void errorMessage(const QString &title, const QString &message, bool isFatal);

	void useMainColorChanged(bool useMainColor);

private:
#ifdef Q_OS_WIN
	QSingleInstance *singleInstance;
#endif

	TrainDataManager *manager;
	QQmlApplicationEngine *engine;
	QPropertySettings *settings;

	TrainControl *trainControl;
	ResultControl *resultControl;
	ConfigWeekControl *configWeekControl;
	CreateTaskControl *createTaskControl;
	ReminderControl *reminderControl;
	MotivateControl *motivateControl;

	bool isValid;
	bool useMainColor;

	void registerTypes();
	void setupEngine();
	void createControls();
	bool loadEngine();
};

#endif // APP_H
