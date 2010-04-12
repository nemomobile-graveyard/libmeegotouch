include(../common_top.pri)
include(../common_mextensions.pri)

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_mmashupcanvasview.cpp \

# unit test and unit classes
HEADERS += \
    ut_mmashupcanvasview.h \

include(../common_bot.pri)
