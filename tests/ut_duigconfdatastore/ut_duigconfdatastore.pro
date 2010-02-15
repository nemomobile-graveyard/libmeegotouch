include(../common_top.pri)
TARGET = ut_duigconfdatastore

INCLUDEPATH += $$DUISRCDIR/mashup/mashup

SOURCES += \
    ut_duigconfdatastore.cpp \
    $$DUISRCDIR/mashup/mashup/duigconfdatastore.cpp

HEADERS += \
    ut_duigconfdatastore.h \
    $$DUISRCDIR/mashup/mashup/duigconfdatastore.h

include(../common_bot.pri)
