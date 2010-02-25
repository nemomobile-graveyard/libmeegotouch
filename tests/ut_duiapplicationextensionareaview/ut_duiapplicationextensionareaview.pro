include(../common_top.pri)
TARGET = ut_duiapplicationextensionareaview
INCLUDEPATH += $$DUISRCDIR/applicationextension $$DUISRCDIR/layout $$DUISRCDIR/style

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_duiapplicationextensionareaview.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiapplicationextensionareaview.h \

include(../common_bot.pri)
