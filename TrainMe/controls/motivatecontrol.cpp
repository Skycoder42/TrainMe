#include "motivatecontrol.h"

MotivateControl::MotivateControl(QObject *parent) :
	ViewControl(parent),
	loader(new GifLoader(this))
{
	connect(this->loader, &GifLoader::gifLoaded,
			this, &MotivateControl::imageLoaded);
}

void MotivateControl::reload()
{
	this->doInit();
}


void MotivateControl::doInit()
{
	this->loader->loadGif("supernatural");
}
