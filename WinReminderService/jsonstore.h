#ifndef JSONSTORE_H
#define JSONSTORE_H

#include <QJsonValue>

class JsonStore
{
public:
	JsonStore(const QString &name);

	QJsonValue load() const;
	void save(const QJsonValue &value);

private:
	QString fileName;
};

#endif // JSONSTORE_H
