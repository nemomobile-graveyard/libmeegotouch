include(../common_top.pri)
TARGET = ut_duiserviceinvoker

INCLUDEPATH += $$DUISRCDIR/corelib/service

TEST_SOURCES = \
    $$DUISRCDIR/corelib/service/duiserviceinvoker.cpp

# unit test and unit
SOURCES += \
    ut_duiserviceinvoker.cpp \
    $$TEST_SOURCES

# unit test and unit
HEADERS += \
    ut_duiserviceinvoker.h \
    $$DUISRCDIR/corelib/service/duiserviceinvoker.h \
    $$DUISRCDIR/corelib/service/duiserviceaction.h

include(../common_bot.pri)
