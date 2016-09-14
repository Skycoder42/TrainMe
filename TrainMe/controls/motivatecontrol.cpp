#include "motivatecontrol.h"
#include <QDebug>

MotivateControl::MotivateControl(QPropertySettings *settings, QObject *parent) :
	ViewControl(parent),
	settings(settings),
	loader(new GifLoader(this)),
	gifTag()
{
	connect(this->loader, &GifLoader::gifLoaded,
			this, &MotivateControl::imageLoaded);

	this->addAction(0, tr("Change search tag"));
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
	if(id == 0)
		emit changeTag();
}
