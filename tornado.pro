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
    cartesiangridtohorizontalslicefilter.cpp \
    flowdatasource.cpp \
    horizontalcontourlinesrenderer.cpp \
    horizontalslicerenderer.cpp \
    horizontalslicetocontourlinemapper.cpp \
    horizontalslicetoimagemapper.cpp \
    horizontalslicetostreamlinemapper.cpp \
    horizontalstreamlinerenderer.cpp \
    main.cpp \
    mainwindow.cpp \
    opengldisplaywidget.cpp \
    datavolumeboundingboxrenderer.cpp

HEADERS += \
    cartesiangridtohorizontalslicefilter.h \
    flowdatasource.h \
    horizontalcontourlinesrenderer.h \
    horizontalslicerenderer.h \
    horizontalslicetocontourlinemapper.h \
    horizontalslicetoimagemapper.h \
    horizontalslicetostreamlinemapper.h \
    horizontalstreamlinerenderer.h \
    mainwindow.h \
    opengldisplaywidget.h \
    datavolumeboundingboxrenderer.h \
    tornadoSrc.h

RESOURCES += \
    Shaders.qrc
