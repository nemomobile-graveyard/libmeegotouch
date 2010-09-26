include(../common_top.pri)
include(../common_mextensions.pri)

QT += core network gui svg dbus

INCLUDEPATH += \
    $$MSRCDIR/extensions/

# unit test and unit classes
SOURCES += \
    ut_mextensionareaview.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionareaview.cpp

# unit test and unit classes
HEADERS += \
    ut_mextensionareaview.h \

include(../common_bot.pri)
