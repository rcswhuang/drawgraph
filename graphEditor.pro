######################################################################
# NOt Automatically generated by qmake (3.0) ?? ?? 25 22:43:40 2017
######################################################################

QT += gui
QT += widgets
TARGET = graphEditor
TEMPLATE = app

INCLUDEPATH += ../include
CONFIG += qt thread all


# Input
HEADERS += ../include/hiconapi.h \
           hgraph.h \
           hgrapheditordoc.h \
           hgrapheditormainwindow.h \
           hgrapheditormgr.h \
           hgrapheditorscene.h \
           hgrapheditorview.h \
           hgraphtreewidget.h \
           hiconlistwidget.h \
           hicontabwidget.h \
           hicontreewidget.h \
           hiconvieweditor.h \
    hgraphapi.h
FORMS += grapheditormainwindow.ui
SOURCES += hgraph.cpp \
           hgrapheditordoc.cpp \
           hgrapheditormainwindow.cpp \
           hgrapheditormgr.cpp \
           hgrapheditorscene.cpp \
           hgrapheaditorview.cpp \
           hgraphtreewidget.cpp \
           hiconlistwidget.cpp \
           hicontabwidget.cpp \
           hicontreewidget.cpp \
           hiconvieweditor.cpp \
           main.cpp
RESOURCES += images.qrc
