#ifndef INTENSENOTIFYMESSAGE_H
#define INTENSENOTIFYMESSAGE_H

#include <QFrame>
#include <QTimer>
#include <QSound>
#include "gifloader.h"

namespace Ui {
	class IntenseNotifyMessage;
}

class IntenseNotifyMessage : public QFrame
{
	Q_OBJECT

public:
	explicit IntenseNotifyMessage(QWidget *parent, const QString &searchTerm);
	~IntenseNotifyMessage();

signals:
	void startTrain();

protected:
	void resizeEvent(QResizeEvent *event) override;

private slots:
	void gifLoaded(QMovie *movie);
	void gifLoadFailed(const QString &error);

	void on_closeButton_clicked();

	void on_muteButton_toggled(bool checked);

private:
	Ui::IntenseNotifyMessage *ui;
	GifLoader *gifLoader;
	QTimer *redTimer;
	QSound *alarmSound;
	bool isRed;

	void updateBlink();
	void reposition();
};

#endif // INTENSENOTIFYMESSAGE_H
