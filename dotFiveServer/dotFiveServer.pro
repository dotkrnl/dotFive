QT += core network
QT -= gui

TARGET = dotFiveServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    fiveserver.cpp \
    fiveserveroptions.cpp \
    fivegame.cpp \
    fivegametask.cpp \
    fivegamemanager.cpp

HEADERS += \
    fiveserver.h \
    fiveserveroptions.h \
    fivegame.h \
    fivegametask.h \
    fivegamemanager.h


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dotFiveLibrary/release/ -ldotFiveLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dotFiveLibrary/debug/ -ldotFiveLibrary
else:unix: LIBS += -L$$OUT_PWD/../dotFiveLibrary/ -ldotFiveLibrary

INCLUDEPATH += $$PWD/../dotFiveLibrary
DEPENDPATH += $$PWD/../dotFiveLibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../dotFiveLibrary/release/libdotFiveLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../dotFiveLibrary/debug/libdotFiveLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../dotFiveLibrary/release/dotFiveLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../dotFiveLibrary/debug/dotFiveLibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../dotFiveLibrary/libdotFiveLibrary.a
