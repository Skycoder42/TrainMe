#ifndef INTENSENOTIFYMESSAGE_H
#define INTENSENOTIFYMESSAGE_H

#include <QFrame>
#include <QTimer>

namespace Ui {
	class IntenseNotifyMessage;
}

class IntenseNotifyMessage : public QFrame
{
	Q_OBJECT

public:
	explicit IntenseNotifyMessage(QWidget *parent = nullptr);
	~IntenseNotifyMessage();

private slots:
	void on_closeButton_clicked();

private:
	Ui::IntenseNotifyMessage *ui;
	QTimer *redTimer;
	bool isRed;

	void updateBlink();
};

#endif // INTENSENOTIFYMESSAGE_H
