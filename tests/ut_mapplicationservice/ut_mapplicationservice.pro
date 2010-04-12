include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/core
TARGET = ut_mapplicationservice

# unit test and unit
SOURCES = \
    ut_mapplicationservice.cpp \
    $$MSRCDIR/corelib/core/mapplicationservice.cpp \

# unit test and unit
HEADERS += \
    ut_mapplicationservice.h 


include(../common_bot.pri)
