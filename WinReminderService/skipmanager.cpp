#include "skipmanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>

SkipManager::SkipManager(QObject *parent) :
	QObject(parent),
	skipDates()
{
	QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	appDataDir.mkpath(QStringLiteral("."));
	QFile skipFile(appDataDir.absoluteFilePath(QStringLiteral("skipcache.dat")));
	skipFile.open(QIODevice::ReadOnly);

	auto doc = QJsonDocument::fromBinaryData(skipFile.readAll());
	if(doc.isNull())
		qWarning("Failed to read skip date cache");
	else {
		auto current = QDate::currentDate();
		auto dates = doc.array();
		foreach (auto value, dates) {
			auto date = QDate::fromString(value.toString(), Qt::ISODate);
			if(date >= current)
				this->skipDates.insert(date);
		}
	}

	skipFile.close();
}

void SkipManager::addSkipDate(const QDate &date)
{
	this->skipDates.insert(date);

	//save
	QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	QSaveFile saveFile(appDataDir.absoluteFilePath(QStringLiteral("skipcache.dat")));
	saveFile.setDirectWriteFallback(true);
	saveFile.open(QIODevice::WriteOnly);

	QJsonArray array;
	foreach (auto date, this->skipDates)
		array.append(date.toString(Qt::ISODate));

	QJsonDocument doc(array);
	auto data = doc.toBinaryData();
	saveFile.write(data);
	saveFile.commit();
}

bool SkipManager::testDateSkipped(const QDate &date) const
{
	return this->skipDates.contains(date);
}
