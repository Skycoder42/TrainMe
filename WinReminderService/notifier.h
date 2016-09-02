#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QWidget>
#include <QSystemTrayIcon>

class Notifier : public QWidget
{
	Q_OBJECT
public:
	explicit Notifier();

public slots:
	void doNotify(bool intense);
	void reloadPermanent();

private slots:
	void openTrainMe();

private:
	QSystemTrayIcon *trayIcon;
};

#endif // NOTIFIER_H
