#ifndef CREATETASKCONTROL_H
#define CREATETASKCONTROL_H

#include <QObject>
#include "viewcontrol.h"

class CreateTaskControl : public ViewControl
{
	Q_OBJECT

	Q_PROPERTY(int freeTasks MEMBER freeTasks NOTIFY freeTasksChanged)
	Q_PROPERTY(bool nextIsAgility MEMBER nextIsAgility NOTIFY nextIsAgilityChanged)

public:
	explicit CreateTaskControl(QObject *parent = nullptr);

public slots:
	void initialize() override;

	void createTask(const QString &name, int baseCount, double factor);

signals:
	void freeTasksChanged();
	void nextIsAgilityChanged();

private slots:
	void freeTasksLoaded(int freeTasks, bool isAgility);

private:
	int freeTasks;
	bool nextIsAgility;
};

#endif // CREATETASKCONTROL_H
