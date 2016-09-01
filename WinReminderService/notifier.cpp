#include "notifier.h"
#include <QApplication>
#include <QMenu>

Notifier::Notifier(QObject *parent) :
	QObject(parent),
	trayIcon(new QSystemTrayIcon(QApplication::windowIcon(), this))
{
	this->trayIcon->setToolTip(QApplication::applicationDisplayName());

	auto menu = new QMenu();
	menu->addAction(tr("Disable Reminders"))->setEnabled(false);
	menu->addAction(tr("Quit"), qApp, &QApplication::quit);
	this->trayIcon->setContextMenu(menu);

	this->trayIcon->show();
}

Notifier::~Notifier()
{
	this->trayIcon->contextMenu()->deleteLater();
}

void Notifier::doNotify(bool intense)
{
	qDebug("HERE");
	this->trayIcon->showMessage("TEST", "TEST");
}
