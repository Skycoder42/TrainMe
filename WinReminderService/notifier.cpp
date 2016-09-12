#include "notifier.h"
#include <QApplication>
#include <QMenu>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include "intensenotifymessage.h"

Notifier::Notifier() :
	QWidget(nullptr),
	trayIcon(new QSystemTrayIcon(QApplication::windowIcon(), this))
{
	this->trayIcon->setToolTip(QApplication::applicationDisplayName());
	connect(this->trayIcon, &QSystemTrayIcon::messageClicked,
			this, &Notifier::openTrainMe);

	auto menu = new QMenu(this);
	auto mainAct = menu->addAction(tr("Open Train-Me!"), this, &Notifier::openTrainMe);
	auto mFont = mainAct->font();
	mFont.setBold(true);
	mainAct->setFont(mFont);
	menu->addSeparator();
	menu->addAction(tr("Disable Reminders"))->setEnabled(false);
	menu->addAction(tr("Quit"), qApp, &QApplication::quit);
	this->trayIcon->setContextMenu(menu);

	this->trayIcon->show();
}

void Notifier::doNotify(bool intense)
{
	if(!this->trayIcon->isVisible())
		this->trayIcon->show();
	if(intense) {
		auto m = new IntenseNotifyMessage(this);
		connect(m, &IntenseNotifyMessage::startTrain,
				this, &Notifier::openTrainMe);
		m->show();
	} else {
		this->trayIcon->showMessage(tr("Do your Training!"),
									tr("It's time for your daily sports training! Klick me to open the Train-Me! App"),
									QSystemTrayIcon::Warning);
	}
}

void Notifier::reloadPermanent()
{

}

void Notifier::openTrainMe()
{
	QDir path = QApplication::applicationDirPath();
	if(!QProcess::startDetached(path.absoluteFilePath(QStringLiteral("TrainMe.exe"))))
		QMessageBox::critical(nullptr, tr("Error"), tr("Failed to start Train-Me!"));
	else
		this->trayIcon->hide();
}
