#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T01:38:38
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MegaMinerAI-Animator
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    CentralPane.cpp \
    PlotWidget.cpp \
    TimeLine.cpp \
    SelectionWidget.cpp \
    TextureViewer.cpp \
    vanim.cpp \
    RightPane.cpp \
    AnimationWidget.cpp

HEADERS  += mainwindow.h \
    CentralPane.h \
    PlotWidget.h \
    TimeLine.h \
    SelectionWidget.h \
    TextureViewer.h \
    vanim.h \
    RightPane.h \
    AnimationWidget.h \
    common.h

QMAKE_CXXFLAGS += -std=c++11
