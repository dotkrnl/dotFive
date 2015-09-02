#-------------------------------------------------
#
# Project created by QtCreator 2015-09-01T19:49:46
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = dotFiveLibrary
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    fiveconnection.cpp \
    fiveboard.cpp \
    fiveconstants.cpp \
    linesocket.cpp

HEADERS += \
    fiveconnection.h \
    fiveboard.h \
    fiveconstants.h \
    linesocket.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
