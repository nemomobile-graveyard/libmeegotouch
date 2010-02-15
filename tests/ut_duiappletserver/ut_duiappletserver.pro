include(../common_top.pri)
TARGET = ut_duiappletserver
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_duiappletserver.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletserver.h \

DEFINES += MY_APPLET_LIBS=\'$$quote(\"./\")\'

include(../common_bot.pri)
