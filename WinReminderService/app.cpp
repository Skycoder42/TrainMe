#include "app.h"
#include <QDebug>

App::App(int argc, char *argv[]) :
	QCoreApplication(argc, argv),
	reminderManager(new ReminderManager(this))
{
	QCoreApplication::setApplicationName(QStringLiteral(TARGET));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));

	connect(reminderManager, &ReminderManager::reminderTriggered, [](bool i) {
		qDebug() << "TRIGGERED! Intense:" << i;
		if(i)
			qApp->quit();
	});
	reminderManager->addReminder(QTime::currentTime().addSecs(5/* * 60*/), false);
	reminderManager->addReminder(QTime::currentTime().addSecs(10/* * 60*/), true);
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
#ifndef QT_NO_DEBUG
	testTimeRing();
#endif
	return app.exec();
}
