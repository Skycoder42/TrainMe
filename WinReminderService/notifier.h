#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QWidget>
#include <QSystemTrayIcon>
#include "skipmanager.h"

class Notifier : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(bool showPermanent READ showPermanent WRITE setShowPermanent NOTIFY showPermanentChanged)
	Q_PROPERTY(QString gifTerm READ gifTerm WRITE setGifTerm NOTIFY gifTermChanged)

public:
	explicit Notifier(SkipManager *skipManager);

	bool showPermanent() const;
	QString gifTerm() const;

public slots:
	void doNotify(bool intense);

	void setShowPermanent(bool showPermanent);
	void setGifTerm(QString gifTerm);

signals:
	void showPermanentChanged(bool showPermanent);
	void gifTermChanged(QString gifTerm);

private slots:
	void openTrainMe();

private:
	SkipManager *skipManager;

	QSystemTrayIcon *trayIcon;
	bool permaShow;
	QString gifTag;
};

#endif // NOTIFIER_H
