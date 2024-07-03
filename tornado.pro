#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tornado
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    filter/cartesiangridtohorizontalslicefilter.cpp \
    source/flowdatasource.cpp \
    renderer/horizontalcontourlinesrenderer.cpp \
    renderer/horizontalslicerenderer.cpp \
    mapper/horizontalslicetocontourlinemapper.cpp \
    mapper/horizontalslicetoimagemapper.cpp \
    mapper/horizontalslicetostreamlinemapper.cpp \
    renderer/horizontalstreamlinerenderer.cpp \
    main.cpp \
    gui/mainwindow.cpp \
    gui/opengldisplaywidget.cpp \
    renderer/datavolumeboundingboxrenderer.cpp

HEADERS += \
    filter/cartesiangridtohorizontalslicefilter.h \
    source/flowdatasource.h \
    renderer/horizontalcontourlinesrenderer.h \
    renderer/horizontalslicerenderer.h \
    mapper/horizontalslicetocontourlinemapper.h \
    mapper/horizontalslicetoimagemapper.h \
    mapper/horizontalslicetostreamlinemapper.h \
    renderer/horizontalstreamlinerenderer.h \
    gui/mainwindow.h \
    gui/opengldisplaywidget.h \
    renderer/datavolumeboundingboxrenderer.h \
    source/tornadoSrc.h

RESOURCES += \
    Shaders.qrc
