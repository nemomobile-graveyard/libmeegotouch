include(../common_top.pri)
include(../common_mextensions.pri)

TEST_SOURCES = \
    $$MSRCDIR/extensions/mashup/mashup/mappletloader.cpp

# unit test and unit
SOURCES += \
    ut_mappletloader.cpp \
    $$TEST_SOURCES

# unit test and unit
HEADERS += \
    ut_mappletloader.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletloader.h

include(../common_bot.pri)
