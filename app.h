#ifndef APP_H
#define APP_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "traindatamanager.h"
#include "trainmodel.h"

#include "controls/traincontrol.h"
#include "controls/resultcontrol.h"

class App : public QGuiApplication
{
	Q_OBJECT

	Q_PROPERTY(double dp MEMBER devicePixels CONSTANT)
	Q_PROPERTY(TrainDataManager *trainManager READ trainManager CONSTANT)

public:
	explicit App(int argc, char *argv[]);

	static App* instance();
	TrainDataManager *trainManager() const;

	bool startupOk();

	Q_INVOKABLE bool testStyle(const QString &styleName) const;

signals:
	void startupCompleted(int startIndex);
	void errorMessage(const QString &title, const QString &message, bool isFatal);

public slots:
	void managerError(QString errorString, bool isFatal);

private:	
	double devicePixels;

	TrainDataManager *manager;
	QQmlApplicationEngine *engine;

	TrainControl *trainControl;
	ResultControl *resultControl;

	bool isValid;

	void registerTypes();
	void setupEngine();
	void createControls();
	bool loadEngine();
};

#endif // APP_H
