#ifndef MOTIVATECONTROL_H
#define MOTIVATECONTROL_H

#include <QObject>
#include "viewcontrol.h"
#include "gifloader.h"
#include <qpropertysettings.h>

class MotivateControl : public ViewControl
{
	Q_OBJECT

	Q_PROPERTY(QString gifTag MEMBER gifTag NOTIFY gifTagChanged)

public:
	explicit MotivateControl(QObject *parent = nullptr);

public slots:
	void reload();

signals:
	void changeTag();
	void imageLoaded(QUrl gifUrl);

	void gifTagChanged(QString gifTag);

protected:
	void doInit() override;
	void actionTriggered(int id) override;

private:
	QPropertySettings *settings;

	GifLoader *loader;
	QString gifTag;
};

#endif // MOTIVATECONTROL_H
