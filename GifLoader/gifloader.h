#ifndef GIFLOADER_H
#define GIFLOADER_H

#include "gifloader_global.h"
#include <QObject>
#include <QUrl>
#include <QMovie>
#include <QTemporaryFile>

class GIFLOADERSHARED_EXPORT GifLoader : public QObject
{
	friend class GifLoaderEngine;
	Q_OBJECT

	Q_PROPERTY(bool loadAsMovie READ loadAsMovie WRITE setLoadAsMovie NOTIFY loadAsMovieChanged)
	Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)

public:
	GifLoader(QObject *parent = nullptr);

	bool loadAsMovie() const;
	QString lastError() const;

public slots:
	void loadGif(const QString &searchTerm);

	void setLoadAsMovie(bool loadAsMovie);

signals:
	void gifLoaded(const QUrl &gifUrl);
	void gifMovieLoaded(QMovie *gifmovie);

	void loadAsMovieChanged(bool loadAsMovie);
	void lastErrorChanged(QString lastError);

private slots:
	void loadDone(const QUrl &gifUrl);
	void downloadDone(QTemporaryFile *downloadFile);
	void loadFailed(const QString &error);

private:
	bool m_loadAsMovie;
	QString m_lastError;
};

#endif // GIFLOADER_H
