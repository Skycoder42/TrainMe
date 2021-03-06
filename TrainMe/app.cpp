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
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif

#include <QDebug>

App::App(int argc, char *argv[]) :
	QGuiApplication(argc, argv),
#ifdef Q_OS_WIN
	singleInstance(new QSingleInstance(this)),
#endif
	manager(nullptr),
	engine(nullptr),
	settings(nullptr),
	trainControl(nullptr),
	resultControl(nullptr),
	configWeekControl(nullptr),
	createTaskControl(nullptr),
	reminderControl(nullptr),
	motivateControl(nullptr),
	isValid(true),
	useMainColor(true)
{
	QGuiApplication::setApplicationName(QStringLiteral(TARGET));
	QGuiApplication::setApplicationVersion(QStringLiteral(VERSION));
	QGuiApplication::setOrganizationName(QStringLiteral(COMPANY));
	QGuiApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QGuiApplication::setApplicationDisplayName(tr("Train Me!"));

#ifdef Q_OS_WIN
	this->singleInstance->setAutoRecovery(true);
	this->singleInstance->setStartupFunction([this](){
#endif
		this->manager = new TrainDataManager(this);
		this->engine = new QQmlApplicationEngine(this);

		this->settings = new QPropertySettings(this);
		this->settings->addProperty(this, "useMainColor");

		this->registerTypes();
		this->setupEngine();

		connect(this->manager, &TrainDataManager::managerReady,
				this, &App::startupCompleted,
				Qt::QueuedConnection);
		connect(this->manager, &TrainDataManager::managerMessage,
				this, &App::errorMessage,
				Qt::QueuedConnection);

		this->createControls();
		this->isValid = this->loadEngine();

#ifdef Q_OS_WIN
		this->singleInstance->setNotifyWindow(this->allWindows().first());
		return this->isValid ? EXIT_SUCCESS : EXIT_FAILURE;
	});
#endif
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

#ifdef Q_OS_WIN
int App::exec()
{
	return this->singleInstance->singleExec();
}
#endif

void App::showToast(QString message, bool isLong)
{
#ifdef Q_OS_ANDROID
	QtAndroid::runOnAndroidThread([=](){
		auto activity = QtAndroid::androidActivity();
		activity.callMethod<void>("showToast",
								  "(Ljava/lang/String;Z)V",
								  QAndroidJniObject::fromString(message).object<jstring>(),
								  (jboolean)isLong);
	});
#else
	Q_UNUSED(isLong);
	qInfo(qPrintable(message));
#endif
}

void App::registerTypes()
{
	qRegisterMetaType<QList<TrainDataManager::ResultInfo>>();
	qRegisterMetaType<ReminderService::ReminderHash>();
}

void App::setupEngine()
{
	//setup coloring mode
#ifdef Q_OS_WIN
	QSettings regSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\DWM", QSettings::NativeFormat);
	QRgb rColor = regSettings.value("ColorizationColor").toUInt();
	this->engine->rootContext()->setContextProperty("hasMainColor", regSettings.contains("ColorizationColor"));
	this->engine->rootContext()->setContextProperty("mainColor", QColor(rColor).name());
#else
	this->engine->rootContext()->setContextProperty("hasMainColor", QVariant(false));
#endif

	//load dpi selector
	QQmlFileSelector *selector = QQmlFileSelector::get(this->engine);
	double dpi = 0.0;
#ifdef Q_OS_ANDROID
	auto activity = QtAndroid::androidActivity();
	dpi = activity.callMethod<jdouble>("getDpiFactor");
#elif defined(Q_OS_WIN)
	auto dpiBase = QGuiApplication::primaryScreen()->logicalDotsPerInch();
	dpi = dpiBase / 96.0;
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
	this->reminderControl = new ReminderControl(this);
	this->motivateControl = new MotivateControl(this->settings, this);
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
	context->setContextProperty("reminderControl", this->reminderControl);
	context->setContextProperty("motivateControl", this->motivateControl);

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
