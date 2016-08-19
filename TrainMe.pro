QT += qml quick quickcontrols2 sql concurrent

CONFIG += c++11

SOURCES += \
    trainmodel.cpp \
    traindatamanager.cpp \
    traintask.cpp \
    app.cpp

RESOURCES += trainme_res.qrc \
	trainme_res_fallbacks.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    trainmodel.h \
    traindatamanager.h \
    traintask.h \
    app.h

DISTFILES += \
    LICENSE
