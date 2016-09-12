#include "app.h"
#include <QCommandLineParser>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

#define QSTR(x) QString(QChar(x))

App::App(int argc, char *argv[]) :
	QApplication(argc, argv),
	singleInstance(new QSingleInstance(this)),
	settings(nullptr),
	skipper(nullptr),
	reminder(nullptr),
	notifier(nullptr)
{
	QCoreApplication::setApplicationName(QStringLiteral(TARGET));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QApplication::setApplicationDisplayName(tr("Train Me! - Windows Reminder Service"));
	QApplication::setWindowIcon(QIcon(QLatin1String(":/icons/main.ico")));
	QApplication::setQuitOnLastWindowClosed(false);

	this->singleInstance->setStartupFunction([this]() {
		return this->startup();
	});
}

App::~App()
{
	delete this->notifier;
}

int App::exec()
{
	QCommandLineParser parser;
	parser.setApplicationDescription(QStringLiteral("The %1 is a helper service to enable train-reminder notifications "
													"on windows. Do not use this service directly, as it is controlled by "
													"the Main Application. If you still want to use it, please remind that "
													"only one of the given options can be used at a time.").arg(QCoreApplication::applicationName()));
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addOption({
						 {"a", "add"},
						 "Used to add a new reminder for the given <time>.",
						 "time"
					 });
	parser.addOption({
						 {"r", "remove"},
						 "Used to remove an existing reminder for <time>.",
						 "time"
					 });
	parser.addOption({
						 {"i", "intense"},
						 "Mark the added reminder as an intense reminder. Do not use this option as standalone option. "
						 "Instead only combine it with the --add option! Other combinations will be ignored."
					 });
	parser.addOption({
						 {"s", "skip"},
						 "Tell the service to not show reminders on <date>.",
						 "date"
					 });
	parser.addOption({
						 {"p", "permanent"},
						 "Tell the service to be permanently shown in the system tray, instead of only "
						 "when notifications occure.",
						 "showPermanent",
						 "true"
					 });
	parser.addOption({
						 {"g", "giftag"},
						 "Tell the service to use the given gif searchtag for intense messages. Leave the "
						 "parameter empty to reset to \"motivation\".",
						 "tag",
						 "motivation"
					 });
	parser.addOption({
						 {"l", "list"},
						 "Writes the services state to the console in JSON-Format. "
						 "Can be used to get the current reminder state."
					 });
	parser.addOption({
						 {"q", "quit"},
						 "Quits the service."
					 });
	parser.process(*this);

	QStringList commandArgs;
	if(parser.isSet("add")) {
		commandArgs.append(QSTR(Add));
		commandArgs.append(parser.value("add"));
		commandArgs.append(parser.isSet("intense") ? "true" :"false");
	} else if(parser.isSet("remove")) {
		commandArgs.append(QSTR(Remove));
		commandArgs.append(parser.value("remove"));
	} else if(parser.isSet("skip")) {
		commandArgs.append(QSTR(Skip));
		commandArgs.append(parser.value("skip"));
	} else if(parser.isSet("permanent")) {
		commandArgs.append(QSTR(Permanent));
		commandArgs.append(parser.value("permanent"));
	} else if(parser.isSet("giftag")) {
		commandArgs.append(QSTR(Giftag));
		commandArgs.append(parser.value("giftag"));
	} else if(parser.isSet("list")) {
		this->printState();
		return EXIT_SUCCESS;
	} else if(parser.isSet("quit"))
		commandArgs.append(QSTR(Quit));

	if(this->singleInstance->process(commandArgs)) {
		if(this->singleInstance->isMaster()) {
			this->startup();
			if(!commandArgs.isEmpty())
				QMetaObject::invokeMethod(this, "handleCommand", Qt::QueuedConnection, Q_ARG(QStringList, commandArgs));
		} else
			return EXIT_SUCCESS;
	} else
		return EXIT_FAILURE;

	return QCoreApplication::exec();
}

void App::handleCommand(const QStringList &args)
{
	if(!args.isEmpty()) {
		if(args[0] == QSTR(Add)) {
			auto time = QTime::fromString(args[1], Qt::ISODate);
			if(time.isValid())
				this->reminder->addReminder(time, args[2] == QStringLiteral("true"));
		} else if(args[0] == QSTR(Remove)) {
			auto time = QTime::fromString(args[1], Qt::ISODate);
			if(time.isValid())
				this->reminder->removeReminder(time);
		} else if(args[0] == QSTR(Skip))
			this->skipper->addSkipDate(QDate::fromString(args[1], Qt::ISODate));
		else if(args[0] == QSTR(Permanent))
			this->notifier->setShowPermanent(args[1] == QStringLiteral("true"));
		else if(args[0] == QSTR(Giftag))
			this->notifier->setGifTerm(args[1]);
		else if(args[0] == QSTR(Quit))
			qApp->quit();
	}
}

int App::startup()
{
	this->settings = new QPropertySettings(this);
	this->skipper = new SkipManager(this);
	this->reminder = new ReminderManager(this);
	this->notifier = new Notifier(this->skipper);

	this->settings->addProperty(this->notifier, "showPermanent");
	this->settings->addProperty(this->notifier, "gifTerm");

	connect(this->singleInstance, &QSingleInstance::instanceMessage,
			this, &App::handleCommand,
			Qt::QueuedConnection);

	connect(reminder, &ReminderManager::reminderTriggered,
			this->notifier, &Notifier::doNotify);

//	//DEBUG
//	this->handleCommand({
//							QSTR(Add),
//							QTime::currentTime().addSecs(3).toString(Qt::ISODate),
//							"false"
//						});
//	this->handleCommand({
//							QSTR(Add),
//							QTime::currentTime().addSecs(6).toString(Qt::ISODate),
//							"true"
//						});

	return EXIT_SUCCESS;
}

void App::printState()
{
	QSettings settings;
	QJsonObject state;

	state[QStringLiteral("permanent")] = settings.value(QStringLiteral("Notifier/showPermanent")).toBool();
	state[QStringLiteral("gifTag")] = settings.value(QStringLiteral("Notifier/gifTerm")).toString();
	state[QStringLiteral("reminders")] = ReminderManager::exportReminders();

	QJsonDocument doc(state);
	auto data = doc.toJson(QJsonDocument::Compact);
	std::cout.write(data.data(), data.size())
			 .flush();
}

int main(int argc, char *argv[])
{
	App app(argc, argv);
	return app.exec();
}
