#include "app.h"
#include <QCommandLineParser>
#include <QDebug>

#define QSTR(x) QString(QChar(x))

App::App(int argc, char *argv[]) :
	QApplication(argc, argv),
	singleInstance(new QSingleInstance(this)),
	manager(nullptr),
	notifier(nullptr)
{
	QCoreApplication::setApplicationName(QStringLiteral(TARGET));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QApplication::setApplicationDisplayName(tr("Train Me! - Windows Reminder Service"));
	QApplication::setWindowIcon(QIcon(QLatin1String(":/icons/main.ico")));

	this->singleInstance->setStartupFunction([this]() {
		return this->startup();
	});
}

int App::exec()
{
	QCommandLineParser parser;
	parser.setApplicationDescription(QStringLiteral("The %1 is a helper service to enable train-reminder notifications "
									 "on windows. Do not use this service directly, as it is controlled by "
									 "the Main Application.").arg(QCoreApplication::applicationName()));
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addOption({
						 {"a", "add"},
						 "Used to add a new reminder for the given <time>",
						 "time"
					 });
	parser.addOption({
						 {"r", "remove"},
						 "Used to remove an existing reminder for <time>",
						 "time"
					 });
	parser.addOption({
						 {"i", "intense"},
						 "Mark the added reminder as an intense reminder"
					 });
	parser.addOption({
						 {"s", "skip"},
						 "Tell the service to not show reminders on <date>",
						 "date"
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
	}

	if(this->singleInstance->process(commandArgs)) {
		if(this->singleInstance->isMaster()) {
			this->startup();
			if(!commandArgs.isEmpty())
				this->handleCommand(commandArgs);
		} else
			return 0;
	} else
		return -1;

	return QCoreApplication::exec();
}

void App::handleCommand(const QStringList &args)
{
	if(!args.isEmpty()) {
		if(args[0] == QSTR(Add)) {
			auto time = QTime::fromString(args[1], Qt::ISODate);
			if(time.isValid())
				this->manager->addReminder(time, args[2] == QStringLiteral("true"));
		} else if(args[0] == QSTR(Remove)) {
			auto time = QTime::fromString(args[1], Qt::ISODate);
			if(time.isValid())
				this->manager->removeReminder(time);
		} else if(args[0] == QSTR(Skip)) {
			qDebug() << "SKIPPED:" << args[1];
		}
	}
}

int App::startup()
{
	this->manager = new ReminderManager(this);
	this->notifier = new Notifier(this);

	connect(this->singleInstance, &QSingleInstance::instanceMessage,
			this, &App::handleCommand);

	connect(manager, &ReminderManager::reminderTriggered,
			this->notifier, &Notifier::doNotify);

	//DEBUG
	this->handleCommand({
							QSTR(Add),
							QTime::currentTime().addSecs(3).toString(Qt::ISODate),
							"false"
						});
	this->handleCommand({
							QSTR(Add),
							QTime::currentTime().addSecs(6).toString(Qt::ISODate),
							"true"
						});

	return 0;
}

void testRound(TimeRing &ring)
{
	auto firstTime = ring.next();
	auto nextTime = firstTime;
	do {
		qDebug() << "TIME:" << nextTime;
		nextTime = ring.next();
	} while(firstTime != nextTime);

	qDebug() << "";
}

void testTimeRing()
{
	//start simple - empty ring
	TimeRing ring;
	testRound(ring);
	Q_ASSERT(!ring.removeTime(QTime()));

	//one element
	ring.insertTime(QTime(1, 0));
	testRound(ring);

	//three elements
	ring.insertTime(QTime(14, 0));
	ring.insertTime(QTime(3, 0));
	testRound(ring);

	//remove elements
	Q_ASSERT(ring.removeTime(QTime(3, 0)));
	Q_ASSERT(!ring.removeTime(QTime(3, 0)));
	testRound(ring);

	//baum elements
	ring.insertTime(QTime(3, 0));
	ring.insertTime(QTime(22, 0));
	testRound(ring);

	//test round
	ring.resetCurrent(QTime(13, 0));
	testRound(ring);
}

int main(int argc, char *argv[])
{
	App app(argc, argv);
//#ifndef QT_NO_DEBUG
//	testTimeRing();
//#endif
	return app.exec();
}
