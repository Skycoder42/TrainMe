#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QSystemTrayIcon>

class Notifier : public QObject
{
	Q_OBJECT
public:
	explicit Notifier(QObject *parent = nullptr);
	~Notifier();

public slots:
	void doNotify(bool intense);

private slots:
	void openTrainMe();

private:
	QSystemTrayIcon *trayIcon;
};

#endif // NOTIFIER_H
