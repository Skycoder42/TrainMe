#ifndef SKIPMANAGER_H
#define SKIPMANAGER_H

#include <QObject>
#include <QDate>
#include <QSet>

class SkipManager : public QObject
{
	Q_OBJECT
public:
	explicit SkipManager(QObject *parent = nullptr);

public:
	void addSkipDate(const QDate &date);
	bool testDateSkipped(const QDate &date = QDate::currentDate()) const;

private:
	QSet<QDate> skipDates;
};

#endif // SKIPMANAGER_H
