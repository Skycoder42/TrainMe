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

	explicit TrainTask(const QString &name,
					   TaskType type,
					   int baseCount,
					   double factor = 1.0,
					   int increment = 0,
					   double penalty = 0.0);

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
	double factor;
	int increment;
	double penalty;
	bool done;
};

#endif // TRAINTASK_H
