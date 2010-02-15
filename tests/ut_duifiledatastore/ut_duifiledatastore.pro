include(../common_top.pri)
TARGET = ut_duifiledatastore
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

SOURCES += \
    ut_duifiledatastore.cpp \

HEADERS += \
    ut_duifiledatastore.h \

include(../common_bot.pri)
