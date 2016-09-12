#include "gifloaderengine.h"
#include <QtNetwork>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

GifLoaderEngine *GifLoaderEngine::_instance = nullptr;

GifLoaderEngine::GifLoaderEngine(QObject *parent) :
	QObject(parent),
	nam(new QNetworkAccessManager(this))
{}

GifLoaderEngine *GifLoaderEngine::instance()
{
	if(!_instance) {
		_instance = new GifLoaderEngine(qApp);
		connect(_instance, &GifLoaderEngine::destroyed, [](){
			_instance = nullptr;
		});
	}
	return _instance;
}

void GifLoaderEngine::loadGif(GifLoader *loader, const QString &tag)
{
	QUrl requestUrl(QStringLiteral("http://api.giphy.com/v1/gifs/random"));
	QUrlQuery query;
	query.addQueryItem("api_key", "dc6zaTOxFJmzC");
	query.addQueryItem("tag", tag);
	requestUrl.setQuery(query);

	QNetworkRequest request(requestUrl);
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

	auto reply = this->nam->get(request);
	connect(reply, &QNetworkReply::finished, this, [=](){
		if(reply->error() != QNetworkReply::NoError)
			loader->loadFailed(reply->errorString());
		else {
			QJsonParseError error;
			QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &error);
			if(error.error != QJsonParseError::NoError)
				loader->loadFailed(error.errorString());
			else {
				auto root = doc.object();
				auto data = root[QStringLiteral("data")].toObject();
				auto url = data[QStringLiteral("image_original_url")].toString();

				QUrl realUrl(url);
				if(realUrl.isValid())
					loader->loadDone(realUrl);
				else
					loader->loadFailed(tr("Failed to find gif url in content data"));
			}
		}

		reply->deleteLater();
	});
}

void GifLoaderEngine::downloadGif(GifLoader *loader, const QUrl &url)
{
	QNetworkRequest request(url);
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

	auto reply = this->nam->get(request);
	connect(reply, &QNetworkReply::finished, this, [=](){
		if(reply->error() != QNetworkReply::NoError)
			loader->loadFailed(reply->errorString());
		else {
			auto tempFile = new QTemporaryFile(loader);
			tempFile->open();
			tempFile->write(reply->readAll());
			tempFile->close();
			loader->downloadDone(tempFile);
		}

		reply->deleteLater();
	});
}
