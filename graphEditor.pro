######################################################################
# NOt Automatically generated by qmake (3.0) ?? ?? 25 22:43:40 2017
######################################################################


TARGET = graphEditor
TEMPLATE = app


INCLUDEPATH += \
    ../include \
    ../include/H5IconGui

CONFIG += qt warn_off thread exceptions

#LIBS datafile.lib
# Input
HEADERS += ../include/hiconapi.h \
           ../include/publicdata.h \
           hgraph.h \
           hgrapheditordoc.h \
           hgrapheditormainwindow.h \
           hgrapheditormgr.h \
           hgrapheditorscene.h \
           hgrapheditorview.h \
           hgraphtreewidget.h \
           hicontabwidget.h \
           hiconvieweditor.h \
           hstation.h \
           hfonthelper.h \
    hiconhelper.h

SOURCES += hgraph.cpp \
           hgrapheditordoc.cpp \
           hgrapheditormainwindow.cpp \
           hgrapheditormgr.cpp \
           hgrapheditorscene.cpp \
           hgrapheditorview.cpp \
           hgraphtreewidget.cpp \
           hicontabwidget.cpp \
           hiconvieweditor.cpp \
           main.cpp \
           hstation.cpp \
           hfonthelper.cpp \
           hgrapheditormainwindowactions.cpp \
    hiconhelper.cpp

FORMS += grapheditormainwindow.ui
RESOURCES += images.qrc \
    pixmaps.qrc

unix{

    LIBS += -L ../lib/ -lH5IconGui
}

win32{
   LIBS += \
        ../lib/H5IconGui.lib \
        ../lib/datafile.lib
}
QT += gui
QT += widgets
QT += xml
