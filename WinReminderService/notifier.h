#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QWidget>
#include <QSystemTrayIcon>

class Notifier : public QWidget
{
	Q_OBJECT
public:
	explicit Notifier(QObject *parent = nullptr);

public slots:
	void doNotify(bool intense);

private slots:
	void openTrainMe();

private:
	QSystemTrayIcon *trayIcon;
};

#endif // NOTIFIER_H
