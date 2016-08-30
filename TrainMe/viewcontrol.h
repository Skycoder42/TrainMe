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
	void initialize();
	void finalize();

	void triggerAction(int index);

signals:
	void menuActionsChanged(QStringList menuActions);

protected:
	TrainDataManager * const manager;

	virtual void doInit() = 0;

	void addAction(int id, const QString &action);
	virtual void actionTriggered(int id);

private slots:
	void completeReset();

private:
	bool isActive;
	QStringList actions;
	QHash<int, int> actionIds;
};

#endif // VIEWCONTROL_H
