#include "gifloader.h"
#include "gifloaderengine.h"
#include <QDebug>
GifLoader::GifLoader(QObject *parent) :
	QObject(parent),
	m_loadAsMovie(false),
	m_lastError()
{}

bool GifLoader::loadAsMovie() const
{
	return this->m_loadAsMovie;
}

QString GifLoader::lastError() const
{
	return this->m_lastError;
}

void GifLoader::loadGif(const QString &searchTerm)
{
	GifLoaderEngine::instance()->loadGif(this, searchTerm);
}

void GifLoader::setLoadAsMovie(bool loadAsMovie)
{
	qDebug() << this->m_loadAsMovie;
	if (this->m_loadAsMovie == loadAsMovie)
		return;

	this->m_loadAsMovie = loadAsMovie;
	emit loadAsMovieChanged(loadAsMovie);
	qDebug() << this->m_loadAsMovie;
}

void GifLoader::loadDone(const QUrl &gifUrl)
{
	qDebug("loadDone");
	if(this->m_loadAsMovie)
		GifLoaderEngine::instance()->downloadGif(this, gifUrl);
	else
		emit gifLoaded(gifUrl);
}

void GifLoader::downloadDone(QTemporaryFile *downloadFile)
{
	qDebug("downloadDone");
	auto movie = new QMovie(downloadFile->fileName(), "gif", this);
	connect(movie, &QMovie::finished, movie, &QMovie::start);
	downloadFile->setParent(movie);
	if(movie->isValid())
		emit gifMovieLoaded(movie);
	else {
		this->m_lastError = tr("Failed to load downloaded gif as movie!");
		emit lastErrorChanged(this->m_lastError);
		movie->deleteLater();
	}
}

void GifLoader::loadFailed(const QString &error)
{
	this->m_lastError = error;
	emit lastErrorChanged(error);
}
