include(../common_top.pri)
TARGET = ut_mserviceinvoker

INCLUDEPATH += $$MSRCDIR/corelib/service

TEST_SOURCES = \
    $$MSRCDIR/corelib/service/mserviceinvoker.cpp

# unit test and unit
SOURCES += \
    ut_mserviceinvoker.cpp \
    $$TEST_SOURCES

# unit test and unit
HEADERS += \
    ut_mserviceinvoker.h \
    $$MSRCDIR/corelib/service/mserviceinvoker.h \
    $$MSRCDIR/corelib/service/mserviceaction.h

include(../common_bot.pri)
