include(../common_top.pri)
TARGET = ut_duimashupcanvasview
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup $$DUISRCDIR/layout $$DUISRCDIR/style

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_duimashupcanvasview.cpp \

# unit test and unit classes
HEADERS += \
    ut_duimashupcanvasview.h \

include(../common_bot.pri)
