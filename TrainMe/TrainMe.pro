QT += qml quick quickcontrols2 sql concurrent

CONFIG += c++11

TARGET = TrainMe
VERSION = 1.0.0

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""

win32 {
	RC_ICONS += ./Resources/main.ico
	QMAKE_TARGET_COMPANY = "Skycoder42"
	QMAKE_TARGET_PRODUCT = "Train Me!"
	QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
	QMAKE_TARGET_COPYRIGHT = "Felix Barz"

	DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
	DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""
} else {
	DEFINES += "COMPANY=\"\\\"Skycoder42\\\"\""
	DEFINES += "DISPLAY_NAME=\"\\\"Train Me!\\\"\""
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
	trainmodel.h \
	traindatamanager.h \
	traintask.h \
	app.h \
	controls/traincontrol.h \
    controls/resultcontrol.h \
	controls/configweekcontrol.h \
    weekconfigmodel.h \
    viewcontrol.h \
    controls/createtaskcontrol.h \
	reminderservice.h

SOURCES += \
	trainmodel.cpp \
	traindatamanager.cpp \
	traintask.cpp \
    app.cpp \
	controls/traincontrol.cpp \
    controls/resultcontrol.cpp \
	controls/configweekcontrol.cpp \
    weekconfigmodel.cpp \
    viewcontrol.cpp \
    controls/createtaskcontrol.cpp \
	reminderservice.cpp

win32 {
	HEADERS += winreminderservice.h
	SOURCES += winreminderservice.cpp
}

RESOURCES += trainme_res.qrc \
	trainme_res_fallbacks.qrc \
    messagebox.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/com/Skycoder42/TrainMe/MainActivity.java \
    android/res/values/styles.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-hdpi/splash.9.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-ldpi/splash.9.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-mdpi/splash.9.png \
    android/res/drawable-xhdpi/icon.png \
    android/res/drawable-xhdpi/splash.9.png \
    android/res/drawable-xxhdpi/icon.png \
    android/res/drawable-xxhdpi/splash.9.png \
    android/res/drawable-xxxhdpi/icon.png \
    android/res/drawable-xxxhdpi/splash.9.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
#ANDROID_EXTRA_LIBS +=
