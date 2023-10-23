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
    appsingleton.cpp \
    copythread.cpp \
    filestree.cpp \
        mainwindow.cpp \
    itemmodel.cpp \
    properties.cpp \
    wsclient.cpp

HEADERS  += mainwindow.h \
    appsingleton.h \
    constvariables.h \
    copythread.h \
    filestree.h \
    itemmodel.h \
    properties.h \
    wsclient.h

FORMS    += mainwindow.ui \
    filestree.ui \
    properties.ui

RESOURCES += \
    icons.qrc
