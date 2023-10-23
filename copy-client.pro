#-------------------------------------------------
#
# Project created by QtCreator 2023-10-18T18:03:41
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = copy-client
TEMPLATE = app


SOURCES += main.cpp\
    copythread.cpp \
        mainwindow.cpp \
    itemmodel.cpp \
    wsclient.cpp

HEADERS  += mainwindow.h \
    copythread.h \
    itemmodel.h \
    wsclient.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
