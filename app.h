#ifndef APP_H
#define APP_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "traindatamanager.h"
#include "trainmodel.h"

class App : public QGuiApplication
{
	Q_OBJECT

	Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)

public:
	explicit App(int argc, char *argv[]);

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
	TrainModel *strengthModel;
	TrainModel *agilityModel;
	QQmlApplicationEngine *engine;

	bool isValid;
	bool loading;

	void setupEngine();
	bool loadEngine();
};

#endif // APP_H
