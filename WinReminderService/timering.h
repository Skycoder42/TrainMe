#ifndef TIMERING_H
#define TIMERING_H

#include <QTime>

class TimeRing
{
public:
	TimeRing();
	~TimeRing();

	void insertTime(const QTime &time);
	bool removeTime(const QTime &time);

	void resetCurrent(const QTime &afterThis = QTime::currentTime());

	QTime current();
	QTime next();

private:
	struct RingElement {
		QTime time;
		RingElement *next;

		RingElement(const QTime &time);
	};

	RingElement *currentElement;

	static bool isBetween(QTime from, QTime to, QTime target);
};

#endif // TIMERING_H
