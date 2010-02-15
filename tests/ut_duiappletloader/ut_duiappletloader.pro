include(../common_top.pri)
TARGET = ut_duiappletloader

INCLUDEPATH += \
    $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/mashup/appletinterface

TEST_SOURCES = \
    $$DUISRCDIR/mashup/mashup/duiappletloader.cpp

# unit test and unit
SOURCES += \
    ut_duiappletloader.cpp \
    $$TEST_SOURCES

# unit test and unit
HEADERS += \
    ut_duiappletloader.h \
    $$DUISRCDIR/mashup/mashup/duiappletloader.h

include(../common_bot.pri)
