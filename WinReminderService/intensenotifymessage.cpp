#include "intensenotifymessage.h"
#include "ui_intensenotifymessage.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QWindow>
#include <dialogmaster.h>

IntenseNotifyMessage::IntenseNotifyMessage(QWidget *parent, const QString &searchTerm) :
	QFrame(parent, Qt::Window |
				   Qt::CustomizeWindowHint |
				   Qt::FramelessWindowHint |
				   Qt::WindowStaysOnTopHint |
				   Qt::WindowDoesNotAcceptFocus),
	ui(new Ui::IntenseNotifyMessage),
	gifLoader(new GifLoader(this)),
	redTimer(new QTimer(this)),
	isRed(false)
{
	this->ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);

	this->gifLoader->setLoadAsMovie(true);
	connect(this->gifLoader, &GifLoader::gifMovieLoaded,
			this, &IntenseNotifyMessage::gifLoaded);
	connect(this->gifLoader, &GifLoader::lastErrorChanged,
			this, &IntenseNotifyMessage::gifLoadFailed);

	connect(this->ui->startTrainButton, &QCommandLinkButton::clicked,
			this, &IntenseNotifyMessage::startTrain);
	connect(this->ui->startTrainButton, &QCommandLinkButton::clicked,
			this, &IntenseNotifyMessage::close);

	connect(this->redTimer, &QTimer::timeout,
			this, &IntenseNotifyMessage::updateBlink);
	this->redTimer->start(300);

	this->gifLoader->loadGif(searchTerm);
	this->reposition();
	QTimer::singleShot(5000, Qt::VeryCoarseTimer, this, &IntenseNotifyMessage::show);
}

IntenseNotifyMessage::~IntenseNotifyMessage()
{
	delete this->ui;
}

void IntenseNotifyMessage::resizeEvent(QResizeEvent *event)
{
	QFrame::resizeEvent(event);
	this->reposition();
}

void IntenseNotifyMessage::gifLoaded(QMovie *movie)
{
	movie->setParent(this);
	movie->start();
	this->ui->motivateLabel->setMovie(movie);
	this->show();
}

void IntenseNotifyMessage::gifLoadFailed(const QString &error)
{
	this->ui->motivateLabel->setText(error);
	this->reposition();
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

void IntenseNotifyMessage::reposition()
{
	auto contentGeom = QApplication::desktop()->availableGeometry();
	auto mRect = this->geometry();
	mRect.moveBottomRight(contentGeom.bottomRight() +
						  QPoint(-10, -10));
	this->move(mRect.topLeft());
}

void IntenseNotifyMessage::on_closeButton_clicked()
{
	if(DialogMaster::question(nullptr,
							  tr("Do you really not want to train right know???"),
							  tr("No Training?!?"),
							  tr("Dismiss Reminder"),
							  QMessageBox::No |
							  QMessageBox::Cancel,
							  QMessageBox::No,
							  QMessageBox::Cancel)
		== QMessageBox::No)
		this->close();
}
