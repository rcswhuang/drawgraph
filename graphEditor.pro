#-------------------------------------------------
#
# Project created by QtCreator 2017-09-18T11:03:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphEditor
TEMPLATE = app


SOURCES += main.cpp\
        hgrapheditormainwindow.cpp \
    hgrapheditormgr.cpp \
    hgrapheditordoc.cpp \
    hgrapheditorview.cpp \
    hgrapheditorscene.cpp \
    hgraphtreewidget.cpp \
    hicontabwidget.cpp \
    hicontreewidget.cpp \
    hiconlistwidget.cpp

HEADERS  += ../include/hiconapi.h \
    hgrapheditormainwindow.h \
    hgrapheditormgr.h \
    hgrapheditordoc.h \
    hgrapheditorview.h \
    hgrapheditorscene.h \
    hgraphtreewidget.h \
    hicontabwidget.h \
    hicontreewidget.h \
    hiconlistwidget.h

FORMS    += grapheditormainwindow.ui

RESOURCES += \
    images.qrc
