#-------------------------------------------------
#
# Project created by QtCreator 2015-06-22T23:41:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FFplay
TEMPLATE = app

CONFIG += static

SOURCES += main.cpp\
        widget.cpp \
    medialineedit.cpp \
    subtitlelineedit.cpp \
    playerprocess.cpp

HEADERS  += widget.h \
    medialineedit.h \
    subtitlelineedit.h \
    playerprocess.h

FORMS    += widget.ui

RESOURCES += \
    icon.qrc

RC_FILE += \
    icon.rc

DISTFILES += \
    icon.ico
