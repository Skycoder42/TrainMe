#include "skipmanager.h"
#include <QJsonArray>

SkipManager::SkipManager(QObject *parent) :
	QObject(parent),
	store(QStringLiteral("skipcache")),
	skipDates()
{
	auto current = QDate::currentDate();
	auto dates = this->store.load().toArray();
	foreach (auto value, dates) {
		auto date = QDate::fromString(value.toString(), Qt::ISODate);
		if(date >= current)
			this->skipDates.insert(date);
	}
}

void SkipManager::addSkipDate(const QDate &date)
{
	this->skipDates.insert(date);

	QJsonArray array;
	foreach (auto date, this->skipDates)
		array.append(date.toString(Qt::ISODate));
	this->store.save(array);
}

bool SkipManager::testDateSkipped(const QDate &date) const
{
	return this->skipDates.contains(date);
}
