include(../common_top.pri)
include(../common_mextensions.pri)

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_mappletclient.cpp \

# unit test and unit classes
HEADERS += \
    ut_mappletclient.h \

DEFINES += MY_APPLET_LIBS=\'$$quote(\"./\")\'

include(../common_bot.pri)
