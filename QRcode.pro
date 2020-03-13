#-------------------------------------------------
#
# Project created by QtCreator 2020-01-07T13:43:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRcode
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

REV_MAJOR_CODE = 0
REV_MINOR_CODE = 1
REV_PATCH_CODE = 1
PROJECT_DIR=$$PWD
include(../QtBaseGeral/base_geral.pri)

SOURCES += \
	gui/sources/main.cpp \
	gui/sources/QRcode.cpp \
	gui/sources/QRcodeComm.cpp \
	\
	uart/sources/serialPort.cpp \
	uart/sources/serialPortDriver.cpp

HEADERS += \
	gui/includes/QRcode.h \
	gui/includes/QRcodeComm.h \
	\
	uart/includes/serialPort.h \
	uart/includes/serialPortDriver.h

FORMS += \
	gui/QRcode.ui

INCLUDEPATH += \
	gui/includes \
	uart/includes

TRANSLATIONS += \
	tr/en_US.ts \
	tr/es_ES.ts \
	tr/pt_BR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	res/QRcode.qrc
