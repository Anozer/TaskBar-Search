#-------------------------------------------------
#
# Project created by QtCreator 2015-11-11T12:41:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TaskBarSearch
TEMPLATE = app

SOURCES += main.cpp\
        taskbarmenu.cpp \
    settings.cpp

HEADERS  += taskbarmenu.h \
    settings.h \
    searchengine.h

FORMS    += taskbarmenu.ui \
    settings.ui

RESOURCES += \
    resources.qrc

RC_ICONS += \
    icons/icon.ico
