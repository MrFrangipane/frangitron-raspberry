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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    track.cpp \
    effects/filter.cpp \
    audioworker.cpp \
    audiomidi.cpp \
    ../include/rtaudio/RtAudio.cpp \
    levelmeter.cpp

HEADERS += \
        mainwindow.h \
    ../include/nlohmann/json.hpp \
    ../include/rtaudio/RtAudio.h \
    typedefs.h \
    track.h \
    effects/filter.h \
    audioworker.h \
    audiomidi.h \
    levelmeter.h

FORMS += \
        mainwindow.ui

LIBS += -lpthread -lasound -lsndfile
