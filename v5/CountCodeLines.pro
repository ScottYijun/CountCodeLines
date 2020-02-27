#-------------------------------------------------
#
# Project created by QtCreator 2020-02-24T22:01:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CountCodeLines
TEMPLATE = app

#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11


SOURCES += main.cpp\
        CItemDelegate.cpp \
        CWorkThread.cpp \
        mainwindow.cpp \
        CTableModel.cpp


HEADERS  += mainwindow.h \
    CCommonHeader.h \
    CItemDelelegate.h \
    CTableModel.h \
    CWorkThread.h

FORMS    += mainwindow.ui

TRANSLATIONS += CountCodeLines_zh.ts
RESOURCES += \
    res.qrc
