include(../common_top.pri)
TARGET = ut_duigconfdatastore

INCLUDEPATH += $$DUISRCDIR/extensions/mashup/mashup

SOURCES += \
    ut_duigconfdatastore.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duigconfdatastore.cpp

HEADERS += \
    ut_duigconfdatastore.h \
    $$DUISRCDIR/extensions/mashup/mashup/duigconfdatastore.h

include(../common_bot.pri)
