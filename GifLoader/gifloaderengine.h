#ifndef GIFLOADERENGINE_H
#define GIFLOADERENGINE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QHash>
#include "gifloader.h"

class GifLoaderEngine : public QObject
{
	Q_OBJECT

public:
	static GifLoaderEngine *instance();

	void loadGif(GifLoader *loader, const QString &tag);
	void downloadGif(GifLoader *loader, const QUrl &url);

private:
	static GifLoaderEngine *_instance;
	QNetworkAccessManager *nam;

	explicit GifLoaderEngine(QObject *parent);
};

#endif // GIFLOADERENGINE_H
