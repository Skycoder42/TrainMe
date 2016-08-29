#include "app.h"
#include <QScreen>
#include <QFileSelector>
#include <QQmlFileSelector>
#include <QQmlContext>
#include <QQuickStyle>
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
	configWeekControl(nullptr),
	createTaskControl(nullptr),
	isValid(false)
{
	this->registerTypes();
	this->setupEngine();

	connect(this->manager, &TrainDataManager::managerReady,
			this, &App::startupCompleted,
			Qt::QueuedConnection);
	connect(this->manager, &TrainDataManager::managerMessage,
			this, &App::errorMessage,
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

bool App::testStyle(const QString &styleName) const
{
	return QQuickStyle::name() == styleName;
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
#ifdef Q_OS_ANDROID
	auto dpi = 3.0;
#elif defined(Q_OS_WIN)
	auto dpiBase = QGuiApplication::primaryScreen()->logicalDotsPerInch();
	auto dpi = dpiBase / 96.0;
#else
#error "Other platforms are currently not supported"
#endif
	if(dpi >= 4.0)
		selector->setExtraSelectors({"xxxhdpi"});
	else if(dpi >= 3.0)
		selector->setExtraSelectors({"xxhdpi"});
	else if(dpi >= 2.0)
		selector->setExtraSelectors({"xhdpi"});
	else if(dpi >= 1.5)
		selector->setExtraSelectors({"hdpi"});
	else if(dpi >= 1.0)
		selector->setExtraSelectors({"mdpi"});
	else
		selector->setExtraSelectors({"ldpi"});
	qDebug() << selector->selector()->allSelectors();
}

void App::createControls()
{
	this->trainControl = new TrainControl(this);
	this->resultControl = new ResultControl(this);
	this->configWeekControl = new ConfigWeekControl(this);
	this->createTaskControl = new CreateTaskControl(this);
}

bool App::loadEngine()
{
	//register c++ models
	auto context = this->engine->rootContext();
	context->setContextProperty("app", this);

	//controls
	context->setContextProperty("trainControl", this->trainControl);
	context->setContextProperty("resultControl", this->resultControl);
	context->setContextProperty("configWeekControl", this->configWeekControl);
	context->setContextProperty("createTaskControl", this->createTaskControl);

	//load main qml
	this->engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
	return !this->engine->rootObjects().isEmpty();
}

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	App app(argc, argv);
	if(app.startupOk())
		return app.exec();
	else
		return EXIT_FAILURE;
}
