include(../common_top.pri)
include(../common_mextensions.pri)

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_mappletserver.cpp \

# unit test and unit classes
HEADERS += \
    ut_mappletserver.h \

DEFINES += MY_APPLET_LIBS=\'$$quote(\"./\")\'

include(../common_bot.pri)
