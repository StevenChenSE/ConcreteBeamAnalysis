#-------------------------------------------------
#
# Project created by QtCreator 2015-12-02T16:37:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RCBeamAnalasys
TEMPLATE = app


SOURCES += main.cpp\
    integral.cpp \
    materials.cpp \
    rcbeam.cpp \
    mainwindow.cpp \
    plotter.cpp \
    plotdialog.cpp

HEADERS  += \
    integral.h \
    materials.h \
    rcbeam.h \
    mainwindow.h \
    plotter.h \
    plotdialog.h

FORMS    += \
    mainwindow.ui \
    plotdialog.ui

DISTFILES +=
