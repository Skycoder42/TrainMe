#ifndef MOTIVATECONTROL_H
#define MOTIVATECONTROL_H

#include <QObject>
#include "viewcontrol.h"
#include "gifloader.h"

class MotivateControl : public ViewControl
{
	Q_OBJECT
public:
	explicit MotivateControl(QObject *parent = nullptr);

public slots:
	void reload();

signals:
	void imageLoaded(QUrl gifUrl);

protected:
	void doInit() override;

private:
	GifLoader *loader;
};

#endif // MOTIVATECONTROL_H
