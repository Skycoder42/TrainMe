#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QWidget>
#include <QSystemTrayIcon>

class Notifier : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(bool showPermanent READ showPermanent WRITE setShowPermanent NOTIFY showPermanentChanged)

public:
	explicit Notifier();

	bool showPermanent() const;

public slots:
	void doNotify(bool intense);

	void setShowPermanent(bool showPermanent);

signals:
	void showPermanentChanged(bool showPermanent);

private slots:
	void openTrainMe();

private:
	QSystemTrayIcon *trayIcon;
	bool permaShow;
};

#endif // NOTIFIER_H
