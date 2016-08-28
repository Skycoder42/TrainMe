#ifndef VIEWCONTROL_H
#define VIEWCONTROL_H

#include <QObject>
#include "traindatamanager.h"

class ViewControl : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QStringList menuActions READ menuActions NOTIFY menuActionsChanged)

public:
	explicit ViewControl(QObject *parent = nullptr);

	QStringList menuActions() const;

public slots:
	virtual void initialize() = 0;

	void triggerAction(int index);

signals:
	void menuActionsChanged(QStringList menuActions);

protected:
	TrainDataManager * const manager;

	void addAction(int id, const QString &action);
	virtual void actionTriggered(int id);

private:
	QStringList actions;
	QHash<int, int> actionIds;
};

#endif // VIEWCONTROL_H
