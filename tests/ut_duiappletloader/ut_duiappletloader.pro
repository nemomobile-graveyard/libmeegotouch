include(../common_top.pri)
include(../common_duiextensions.pri)

TEST_SOURCES = \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletloader.cpp

# unit test and unit
SOURCES += \
    ut_duiappletloader.cpp \
    $$TEST_SOURCES

# unit test and unit
HEADERS += \
    ut_duiappletloader.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletloader.h

include(../common_bot.pri)
