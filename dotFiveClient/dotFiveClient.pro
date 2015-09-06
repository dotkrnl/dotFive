#-------------------------------------------------
#
# Project created by QtCreator 2015-09-01T19:53:43
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dotFiveClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainscene.cpp \
    fiveboardview.cpp \
    filevisual.cpp \
    welcomescene.cpp \
    settingscene.cpp \
    disconnectedscene.cpp \
    startscene.cpp \
    requestscene.cpp

HEADERS  += mainwindow.h \
    mainscene.h \
    fiveboardview.h \
    fivevisual.h \
    welcomescene.h \
    settingscene.h \
    disconnectedscene.h \
    startscene.h \
    requestscene.h

FORMS    += mainwindow.ui \
    mainscene.ui \
    welcomescene.ui \
    settingscene.ui \
    disconnectedscene.ui \
    startscene.ui \
    requestscene.ui


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

RESOURCES += \
    fonts.qrc \
    sounds.qrc

ICON = dotFive.icns
