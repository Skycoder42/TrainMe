#include "app.h"
#include <QScreen>
#include <QFileSelector>
#include <QQmlFileSelector>
#include <QQmlContext>
#ifdef Q_OS_WIN
#include <QSettings>
#include <QColor>
#endif

#include <QDebug>

App::App(int argc, char *argv[]) :
	QGuiApplication(argc, argv),
	manager(new TrainDataManager(this)),
	engine(new QQmlApplicationEngine(this)),
	trainControl(nullptr),
	resultControl(nullptr),
	isValid(false)
{
	this->registerTypes();
	this->setupEngine();

	connect(this->manager, &TrainDataManager::managerReady,
			this, &App::startupCompleted,
			Qt::QueuedConnection);
	connect(this->manager, &TrainDataManager::managerError,
			this, &App::managerError,
			Qt::QueuedConnection);

	this->createControls();
	if(!this->loadEngine())
		return;

	this->isValid = true;
}

App *App::instance()
{
	return static_cast<App*>(QCoreApplication::instance());
}

TrainDataManager *App::trainManager() const
{
	return this->manager;
}

bool App::startupOk()
{
	return this->isValid;
}

void App::managerError(QString errorString, bool isFatal)
{
	emit errorMessage(isFatal ? tr("Fatal Database Error!") : tr("Database Error"), errorString, isFatal);
}

void App::registerTypes()
{
	qRegisterMetaType<QList<TrainDataManager::ResultInfo>>();
}

void App::setupEngine()
{
	//setup coloring mode
#ifdef Q_OS_WIN
	QSettings regSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\DWM", QSettings::NativeFormat);
	QRgb rColor = regSettings.value("ColorizationColor").toUInt();
	this->engine->rootContext()->setContextProperty("hasMainColor", QVariant(true));
	this->engine->rootContext()->setContextProperty("mainColor", QColor(rColor).name());
#else
	this->engine->rootContext()->setContextProperty("hasMainColor", QVariant(false));
#endif

	//load dpi selector
	QQmlFileSelector *selector = QQmlFileSelector::get(this->engine);
	auto dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
	if(dpi > 480)
		selector->setExtraSelectors({"xxxhdpi"});
	else if(dpi > 320)
		selector->setExtraSelectors({"xxhdpi"});
	else if(dpi > 240)
		selector->setExtraSelectors({"xhdpi"});
	else if(dpi > 160)
		selector->setExtraSelectors({"hdpi"});
	else
		selector->setExtraSelectors({"mdpi"});
	//TODO DPI fails on android -> WHY?
}

void App::createControls()
{
	this->trainControl = new TrainControl(this);
	this->resultControl = new ResultControl(this);
}

bool App::loadEngine()
{
	//register c++ models
	auto context = this->engine->rootContext();
	context->setContextProperty("app", this);

	//controls
	context->setContextProperty("trainControl", this->trainControl);
	context->setContextProperty("resultControl", this->resultControl);

	//load main qml
	this->engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
	return !this->engine->rootObjects().isEmpty();
}

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	App app(argc, argv);
	if(app.startupOk()) {
		QMetaObject::invokeMethod(app.trainManager(), "initManager", Qt::QueuedConnection);
		return app.exec();
	} else
		return EXIT_FAILURE;
}
