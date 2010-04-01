include(../common_top.pri)
include(../common_duiextensions.pri)

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_duiappletserver.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletserver.h \

DEFINES += MY_APPLET_LIBS=\'$$quote(\"./\")\'

include(../common_bot.pri)
