#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T15:24:46
#
#-------------------------------------------------

QT       += core gui
QT       += testlib
QMAKE_CXXFLAGS_RELEASE += -O3
CONFIG += c++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = foot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    player.cpp \
    composition.cpp \
    composition_helper.cpp \
    player_helper.cpp \
    formation.cpp \
    predicate.cpp \
    algorithmoption.cpp \
    elitism.cpp \
    biasedroulette.cpp \
    tournament.cpp \
    geneticalgorithm.cpp \
    geneticFootAlgorithm.cpp

HEADERS  += mainwindow.h \
    player.h \
    composition.h \
    composition_helper.h \
    player_helper.h \
    formation.h \
    predicate.h \
    algorithmoption.h \
    elitism.h \
    biasedroulette.h \
    hashingextension.h \
    tournament.h \
    geneticalgorithm.h \
    geneticFootAlgorithm.h

FORMS    += mainwindow.ui
