#ifndef TRAINTASK_H
#define TRAINTASK_H

#include <QObject>

class TrainTask : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString name READ name CONSTANT)
	Q_PROPERTY(TaskType taskType READ taskType CONSTANT)
	Q_PROPERTY(int trainCount READ trainCount CONSTANT)
	Q_PROPERTY(bool done READ isDone WRITE setDone NOTIFY doneChanged)

public:
	enum TaskType {
		StrengthTask,
		AgilityTask
	};
	Q_ENUM(TaskType)

	explicit TrainTask(const QString &name, TaskType type, int baseCount, int increment);

	QString name() const;
	int trainCount() const;
	bool isDone() const;
	TaskType taskType() const;

public slots:
	bool setDone(bool done);

signals:
	void doneChanged(bool done);

private:
	QString trainName;
	TaskType type;
	int baseCount;
	int increment;
	bool done;
};

#endif // TRAINTASK_H
