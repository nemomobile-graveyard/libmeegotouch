include(../common_top.pri)
include(../common_duiextensions.pri)

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_duiapplicationextensionareaview.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiapplicationextensionareaview.h \

include(../common_bot.pri)
