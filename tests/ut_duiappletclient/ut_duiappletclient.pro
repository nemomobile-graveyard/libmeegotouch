include(../common_top.pri)
TARGET = ut_duiappletclient
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_duiappletclient.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletclient.h \

DEFINES += MY_APPLET_LIBS=\'$$quote(\"./\")\'

include(../common_bot.pri)
