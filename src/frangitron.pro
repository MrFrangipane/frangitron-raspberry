#-------------------------------------------------
#
# Project created by QtCreator 2018-11-10T12:21:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = frangitron
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
DEFINES += __LINUX_ALSA__
DEFINES += RTAUDIO_DEBUG

CONFIG += debug

SOURCES += \
    ../include/rtaudio/RtAudio.cpp \
    main.cpp \
    audio/engine.cpp \
    audio/levelmeter.cpp \
    audio/filter.cpp \
    audio/compressor.cpp \
    ui/mainwindow.cpp \
    ui/audioworker.cpp \
    ui/abstractwidget.cpp \
    ui/levelmeterwidget.cpp \
    ui/filterwidget.cpp \
    ui/compwidget.cpp

HEADERS += \
    ../include/nlohmann/json.hpp \
    ../include/rtaudio/RtAudio.h \
    typedefs.h \
    audio/engine.h \
    audio/levelmeter.h \
    audio/filter.h \
    audio/compressor.h \
    ui/helpers.h \
    ui/mainwindow.h \
    ui/audioworker.h \
    ui/abstractwidget.h \
    ui/levelmeterwidget.h \
    ui/filterwidget.h \
    ui/compwidget.h

FORMS += \
    ui/mainwindow.ui

LIBS += -lpthread -lasound -lsndfile
QMAKE_CXXFLAGS *= -O3
