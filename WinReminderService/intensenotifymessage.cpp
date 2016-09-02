#include "intensenotifymessage.h"
#include "ui_intensenotifymessage.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QWindow>
#include <QMessageBox>//TODO dialog master

IntenseNotifyMessage::IntenseNotifyMessage(QWidget *parent) :
	QFrame(parent, Qt::Window |
				   Qt::CustomizeWindowHint |
				   Qt::FramelessWindowHint |
				   Qt::WindowStaysOnTopHint |
				   Qt::WindowDoesNotAcceptFocus),
	ui(new Ui::IntenseNotifyMessage),
	redTimer(new QTimer(this)),
	isRed(false)
{
	this->ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->adjustSize();

	connect(this->ui->startTrainButton, &QCommandLinkButton::clicked,
			this, &IntenseNotifyMessage::startTrain);
	connect(this->ui->startTrainButton, &QCommandLinkButton::clicked,
			this, &IntenseNotifyMessage::close);

	connect(this->redTimer, &QTimer::timeout,
			this, &IntenseNotifyMessage::updateBlink);
	this->redTimer->start(300);

	auto contentGeom = QApplication::desktop()->availableGeometry();
	auto mRect = this->geometry();
	mRect.moveBottomRight(contentGeom.bottomRight() +
						  QPoint(-10, -10));
	this->move(mRect.topLeft());
}

IntenseNotifyMessage::~IntenseNotifyMessage()
{
	delete this->ui;
}

void IntenseNotifyMessage::updateBlink()
{
	this->isRed = !this->isRed;
	this->setStyleSheet(QStringLiteral("#IntenseNotifyMessage {\n"
									   "	border: 1px solid %1;\n"
									   "	background: #FFFFFF;\n"
									   "}\n"
									   "QFrame[frameShape=\"4\"] {\n"
									   "	color: %1;\n"
									   "}\n"
									   "QLabel {\n"
									   "	color: #424242;\n"
									   "}\n"
									   "QCommandLinkButton {\n"
									   "	color: #424242;\n"
									   "}")
						.arg(this->isRed ? QStringLiteral("#DF0101") : QStringLiteral("#585858")));
}

void IntenseNotifyMessage::on_closeButton_clicked()
{
	if(QMessageBox::question(nullptr,
							 tr("Dismiss Reminder"),
							 tr("Do you really not want to train right know???"),
							 QMessageBox::No,
							 QMessageBox::Cancel)
		== QMessageBox::No)
		this->close();
}
