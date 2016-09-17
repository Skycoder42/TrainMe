#include "motivatecontrol.h"
#include <QClipboard>
#include <QDebug>
#include <QGuiApplication>

MotivateControl::MotivateControl(QPropertySettings *settings, QObject *parent) :
	ViewControl(parent),
	settings(settings),
	loader(new GifLoader(this)),
	gifTag(),
	lastUrl()
{
	connect(this->loader, &GifLoader::gifLoaded,
			this, &MotivateControl::imageLoaded);
	connect(this->loader, &GifLoader::gifLoaded, this, [this](QUrl url){
		this->lastUrl = url;
	});

	this->addAction(0, tr("Change search tag"));
	this->addAction(1, tr("Copy GIF-Link"));
	this->settings->addProperty(this, "gifTag");
}

void MotivateControl::reload()
{
	this->doInit();
}

void MotivateControl::doInit()
{
	this->loader->loadGif(this->gifTag);
}

void MotivateControl::actionTriggered(int id)
{
	switch(id) {
	case 0:
		emit changeTag();
		break;
	case 1:
		QGuiApplication::clipboard()->setText(this->lastUrl.toString());
		break;
	}
}
