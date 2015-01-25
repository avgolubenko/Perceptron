#-------------------------------------------------
#
# Project created by QtCreator 2015-01-03T21:38:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimplePerceptron
TEMPLATE = app


SOURCES += main.cpp\
        main-window.cpp \
    worker-thread.cpp \
    neuron.cpp \
    perceptron.cpp

HEADERS  += main-window.h \
    worker-thread.h \
    neuron.h \
    perceptron.h

FORMS    += main-window.ui

RESOURCES += \
    simpleperceptron.qrc
