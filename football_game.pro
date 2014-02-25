#-------------------------------------------------
#
# Project created by QtCreator 2013-11-17T16:37:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paper_football
TEMPLATE = app


SOURCES +=\
    src/ball.cpp \
    src/football_field.cpp \
    src/fpoint.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS  += \
    src/ball.h \
    src/football_field.h \
    src/fpoint.h \
    src/mainwindow.h

FORMS    += \
    forms/mainwindow.ui
