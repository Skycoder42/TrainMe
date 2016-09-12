#include "jsonstore.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDebug>

#define SPEC_VAL_KEY QStringLiteral("_")

JsonStore::JsonStore(const QString &name) :
	fileName()
{
	QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	appDataDir.mkpath(QStringLiteral("."));
	this->fileName = appDataDir.absoluteFilePath(name + QStringLiteral(".dat"));
}

QJsonValue JsonStore::load() const
{
	QJsonValue resValue = QJsonValue::Undefined;

	QFile loadFile(this->fileName);
	if(loadFile.exists()) {
		loadFile.open(QIODevice::ReadOnly);
		auto data = loadFile.readAll();
		loadFile.close();

		auto doc = QJsonDocument::fromBinaryData(data);
		if(doc.isNull())
			qWarning() << "Failed to load data from" << this->fileName;
		else {
			auto obj = doc.object();
			resValue = obj.value(SPEC_VAL_KEY);
		}
	}

	return resValue;
}

void JsonStore::save(const QJsonValue &value)
{
	QSaveFile saveFile(this->fileName);
	saveFile.setDirectWriteFallback(true);
	saveFile.open(QIODevice::WriteOnly);

	QJsonObject obj;
	obj[SPEC_VAL_KEY] = value;
	QJsonDocument doc(obj);
	auto data = doc.toBinaryData();
	saveFile.write(data);
	if(!saveFile.commit())
		qCritical() << "Failed to save data!!!";
}
