#-------------------------------------------------
#
# Project created by QtCreator 2017-06-25T22:01:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CountCodeLines
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    line.cpp

HEADERS  += mainwindow.h \
    line.h \

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
