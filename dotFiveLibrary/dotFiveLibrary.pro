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
    lineconnection.cpp \
    fiveconnection.cpp \
    fiveboard.cpp \
    fiveconstants.cpp

HEADERS += \
    lineconnection.h \
    fiveconnection.h \
    fiveboard.h \
    fiveconstants.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
