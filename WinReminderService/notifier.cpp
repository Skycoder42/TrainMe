#include "notifier.h"
#include <QApplication>
#include <QMenu>

Notifier::Notifier(QObject *parent) :
	QObject(parent),
	trayIcon(new QSystemTrayIcon(QApplication::windowIcon(), this))
{
	this->trayIcon->setToolTip(QApplication::applicationDisplayName());
	connect(this->trayIcon, &QSystemTrayIcon::messageClicked,
			this, &Notifier::openTrainMe);

	auto menu = new QMenu();
	auto mainAct = menu->addAction(tr("Open Train-Me!"), this, &Notifier::openTrainMe);
	auto mFont = mainAct->font();
	mFont.setBold(true);
	mainAct->setFont(mFont);
	menu->addSeparator();
	menu->addAction(tr("Disable Reminders"))->setEnabled(false);
	menu->addAction(tr("Quit"), qApp, &QApplication::quit);
	this->trayIcon->setContextMenu(menu);

	this->trayIcon->show();//TODO allow hiding
}

Notifier::~Notifier()
{
	this->trayIcon->contextMenu()->deleteLater();
}

void Notifier::doNotify(bool intense)
{
	if(intense)
		qDebug("INTENSE!!!");
	else {
		this->trayIcon->showMessage(tr("Do your Training!"),
									tr("It's time for your daily sports training! Klick me to open the Train-Me! App"),
									QSystemTrayIcon::Warning);
	}
}

void Notifier::openTrainMe()
{
	qDebug("Opening Train-Me!...");
}
