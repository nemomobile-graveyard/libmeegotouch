include(../common_top.pri)
TARGET = ut_mgconfdatastore

INCLUDEPATH += $$MSRCDIR/extensions/mashup/mashup

SOURCES += \
    ut_mgconfdatastore.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mgconfdatastore.cpp

HEADERS += \
    ut_mgconfdatastore.h \
    $$MSRCDIR/extensions/mashup/mashup/mgconfdatastore.h

include(../common_bot.pri)
