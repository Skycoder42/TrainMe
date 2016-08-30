#include "timering.h"

TimeRing::TimeRing() :
	currentElement(nullptr)
{}

TimeRing::~TimeRing()
{
	if(this->currentElement) {
		auto element = this->currentElement->next;
		this->currentElement->next = nullptr;
		while(element != nullptr) {
			auto oElem = element;
			element = oElem->next;
			delete oElem;
		}
	}
}

void TimeRing::insertTime(const QTime &time)
{
	if(!this->currentElement){
		this->currentElement = new RingElement(time);
		this->currentElement->next = this->currentElement;
	} else {
		auto prevElem = this->currentElement;
		auto nextElem = this->currentElement->next;

		while(nextElem != this->currentElement) {
			if(time == prevElem->time || time == nextElem->time)
				return;//BREAK here. duplicates are not allowed
			if(isBetween(prevElem->time, nextElem->time, time)) {//between 2 times -> must belong there
				auto newElem = new RingElement(time);
				prevElem->next = newElem;
				newElem->next = nextElem;
				return;//done here
			}

			//not between -> check next pair
			prevElem = nextElem;
			nextElem = nextElem->next;
		}

		//not between any other -> must be before current
		auto newElem = new RingElement(time);
		prevElem->next = newElem;// prev is time before current
		newElem->next = nextElem;// next is current
	}
}

bool TimeRing::removeTime(const QTime &time)
{
	if(this->currentElement) {
		if(this->currentElement->next == this->currentElement) {//only 1 element there
			if(this->currentElement->time == time) {
				delete this->currentElement;
				this->currentElement = nullptr;
				return true;
			}
		} else {//multiple elements
			RingElement *prev;
			auto cur = this->currentElement;
			do {
				prev = cur;
				cur = cur->next;
				if(cur->time == time) {
					prev->next = cur->next;
					if(this->currentElement == cur)
						this->currentElement = cur->next;
					delete cur;
					return true;
				}
			} while(cur != this->currentElement);
		}
	}

	return false;
}

void TimeRing::resetCurrent(const QTime &afterThis)
{
	if(this->currentElement) {
		auto prev = this->currentElement;
		auto next = prev->next;

		while(next != this->currentElement) {
			if(isBetween(prev->time, next->time, afterThis)) {
				this->currentElement = next;
				return;
			}
			prev = next;
			next = next->next;
		}
	}
}

QTime TimeRing::current()
{
	if(!this->currentElement)
		return QTime();
	else
		return this->currentElement->time;
}

QTime TimeRing::next()
{
	if(!this->currentElement)
		return QTime();
	else {
		this->currentElement = this->currentElement->next;
		return this->currentElement->time;
	}
}

bool TimeRing::isBetween(QTime from, QTime to, QTime target)
{
	if(from < to)
		return target > from && target < to;
	else
		return target > from || target < to;
}



TimeRing::RingElement::RingElement(const QTime &time) :
	time(time),
	next(nullptr)
{}
