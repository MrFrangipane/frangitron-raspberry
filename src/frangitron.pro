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
    ../include/rtmidi/RtMidi.cpp \
    main.cpp \
    engine/engine.cpp \
    audio/_samplemeter.cpp \
    audio/_samplefilter.cpp \
    audio/levelmeter.cpp \
    audio/filter.cpp \
    audio/compressor.cpp \
    ui/mainwindow.cpp \
    ui/abstractwidget.cpp \
    ui/levelmeterwidget.cpp \
    ui/filterwidget.cpp \
    ui/compwidget.cpp \
    midi/encoder.cpp \
    shared/masterclock.cpp \
    ui/sequencerwidget.cpp \
    audio/_aweighting.cpp \
    audio/kicksynth.cpp \
    ui/kickwidget.cpp \
    audio/_envelope.cpp \
    audio/recorder.cpp \
    shared/configuration.cpp \
    audio/sampleplayer.cpp \
    ui/sampleplayerwidget.cpp \
    audio/_samplebank.cpp \
    audio/djdeck.cpp \
    audio/_djtrackbank.cpp


HEADERS += \
    ../include/nlohmann/json.hpp \
    ../include/rtaudio/RtAudio.h \
    ../include/rtmidi/RtMidi.h \
    shared/constants.h \
    shared/typedefs.h \
    shared/structures.h \
    shared/audiohelpers.h \
    shared/uihelpers.h \
    engine/engine.h \
    audio/abstractmodule.h \
    audio/_samplemeter.h \
    audio/_samplefilter.h \
    audio/levelmeter.h \
    audio/filter.h \
    audio/compressor.h \
    ui/mainwindow.h \
    ui/engineworker.h \
    ui/abstractwidget.h \
    ui/levelmeterwidget.h \
    ui/filterwidget.h \
    ui/compwidget.h \
    midi/encoder.h \
    shared/masterclock.h \
    ui/sequencerwidget.h \
    audio/_aweighting.h \
    audio/kicksynth.h \
    ui/kickwidget.h \
    audio/_envelope.h \
    audio/recorder.h \
    shared/configuration.h \
    audio/sampleplayer.h \
    ui/sampleplayerwidget.h \
    audio/_samplebank.h \
    audio/djdeck.h \
    audio/_djtrackbank.h

RESOURCES += \
    images.qrc

FORMS += \
    ui/mainwindow.ui

LIBS += -lpthread -lasound -lsndfile

linux {
    contains(QMAKE_HOST.arch, arm.*): {
        DEFINES += RASPBERRYPI
        QMAKE_CXXFLAGS *= -g -O3
    } else {
        QMAKE_CXXFLAGS *= -g -O0
    }
}
