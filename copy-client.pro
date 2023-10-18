#-------------------------------------------------
#
# Project created by QtCreator 2023-10-18T18:03:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = copy-client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    itemmodel.cpp

HEADERS  += mainwindow.h \
    itemmodel.h

FORMS    += mainwindow.ui
