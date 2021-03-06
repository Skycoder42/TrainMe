#include "winreminderservice.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

#define REMINDER_EXE_NAME QStringLiteral("WinReminderService")

WinReminderService::WinReminderService(QObject *parent) :
	ReminderService(parent)
{
	//make shure the service is running if active
	QSettings regEdit(QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
	auto isActive = regEdit.contains(REMINDER_EXE_NAME);
	if(isActive)
		QProcess::startDetached(this->remPath());

#ifndef QT_NO_DEBUG
	connect(qApp, &QCoreApplication::aboutToQuit, this, [this](){
		QProcess::startDetached(this->remPath(), {QStringLiteral("--quit")});
	});
#endif
}

void WinReminderService::loadState()
{
	QSettings regEdit(QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
	auto isActive = regEdit.contains(REMINDER_EXE_NAME);

	auto serviceProc = new QProcess(this);
	serviceProc->setProgram(this->remPath());
	serviceProc->setArguments({QStringLiteral("--list")});
	serviceProc->setProcessChannelMode(QProcess::ForwardedErrorChannel);
	connect(serviceProc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
			this, [=](int exitCode, QProcess::ExitStatus exitStatus){
		if(exitStatus == QProcess::NormalExit && exitCode == EXIT_SUCCESS) {
			auto data = serviceProc->readAll();

			QJsonParseError error;
			auto doc = QJsonDocument::fromJson(data, &error);
			if(error.error == QJsonParseError::NoError) {
				auto root = doc.object();

				ReminderHash reminders;
				auto array = root[QStringLiteral("reminders")].toArray();
				foreach(auto value, array) {
					auto remObj = value.toObject();
					reminders.insert(QTime::fromString(remObj[QStringLiteral("time")].toString(), Qt::ISODate),
									 remObj[QStringLiteral("intense")].toBool());
				}

				emit stateLoaded(isActive,
								 root[QStringLiteral("permanent")].toBool(),
								 root[QStringLiteral("gifTag")].toString(),
								 reminders);
			}
		}

		serviceProc->deleteLater();
	});

	serviceProc->start();
}

void WinReminderService::setRemindersActive(bool active)
{
	QSettings regEdit(QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
	if(active) {
		regEdit.setValue(REMINDER_EXE_NAME, QDir::toNativeSeparators(this->remPath()));
		QProcess::startDetached(this->remPath());
	} else {
		regEdit.remove(REMINDER_EXE_NAME);
		QProcess::startDetached(this->remPath(), {QStringLiteral("--quit")});
	}
}

void WinReminderService::addReminder(const QTime &time, bool intense)
{
	QStringList args;
	args.append(QStringLiteral("--add"));
	args.append(time.toString(Qt::ISODate));
	if(intense)
		args.append(QStringLiteral("--intense"));
	QProcess::startDetached(this->remPath(), args);
}

void WinReminderService::removeReminder(const QTime &time)
{
	QStringList args;
	args.append(QStringLiteral("--remove"));
	args.append(time.toString(Qt::ISODate));
	QProcess::startDetached(this->remPath(), args);
}

void WinReminderService::skipReminder(const QDate &skipDate)
{
	QStringList args;
	args.append(QStringLiteral("--skip"));
	args.append(skipDate.toString(Qt::ISODate));
	QProcess::startDetached(this->remPath(), args);
}

void WinReminderService::setPermanent(bool permanent)
{
	QStringList args;
	args.append(QStringLiteral("--permanent"));
	args.append(permanent ? QStringLiteral("true") : QStringLiteral("false"));
	QProcess::startDetached(this->remPath(), args);
}

void WinReminderService::setGifTag(QString gifTag)
{
	QStringList args;
	args.append(QStringLiteral("--giftag"));
	args.append(gifTag);
	QProcess::startDetached(this->remPath(), args);
}

QString WinReminderService::remPath() const
{
#ifdef QT_NO_DEBUG
	QDir appDir(QCoreApplication::applicationDirPath());
	return appDir.absoluteFilePath(REMINDER_EXE_NAME + QStringLiteral(".exe"));
#else
	QDir appDir(QCoreApplication::applicationDirPath());
	appDir.cd(QStringLiteral("../../DEBUG_SERVICE_LOCATION"));
	return appDir.absoluteFilePath(REMINDER_EXE_NAME + QStringLiteral(".exe"));
#endif
}
