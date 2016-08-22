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

	Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
	Q_PROPERTY(TrainDataManager *trainManager READ trainManager CONSTANT)

public:
	explicit App(int argc, char *argv[]);

	static App* instance();
	TrainDataManager *trainManager() const;

	bool startupOk();
	bool isLoading() const;

signals:
	void errorMessage(const QString &title, const QString &message, bool isFatal);

	void loadingChanged(bool loading);

public slots:
	void managerReady(QString errorString);
	void managerError(QString errorString, bool isFatal);

private:
	TrainDataManager *manager;
	QQmlApplicationEngine *engine;

	TrainControl *trainControl;
	ResultControl *resultControl;

	bool isValid;
	bool loading;

	void registerTypes();
	void setupEngine();
	void createControls();
	bool loadEngine();
};

#endif // APP_H
