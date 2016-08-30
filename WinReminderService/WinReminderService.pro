TEMPLATE = app
QT += core
QT -= gui

CONFIG += c++11

TARGET = WinReminderService
VERSION = 1.0.0

DEFINES += "TARGET=\\\"TrainMeWinReminderService\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""

RC_ICONS += ./main.ico
QMAKE_TARGET_COMPANY = "Skycoder42"
QMAKE_TARGET_PRODUCT = "Train Me! — Windows Reminder Service"
QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
QMAKE_TARGET_COPYRIGHT = "Felix Barz"

DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""

HEADERS += \
    app.h \
    remindermanager.h \
    timering.h

SOURCES += \
    app.cpp \
    remindermanager.cpp \
    timering.cpp
