#-------------------------------------------------
#
# Project created by QtCreator 2018-12-06T07:21:34
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpfile
TEMPLATE = app


SOURCES += main.cpp \
    serverwidget.cpp \
    cliwidget.cpp

HEADERS  += \
    serverwidget.h \
    cliwidget.h

FORMS    += \
    serverwidget.ui \
    cliwidget.ui

CONFIG  += C++11
