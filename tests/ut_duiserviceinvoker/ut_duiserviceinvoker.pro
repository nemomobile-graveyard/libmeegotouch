include(../common_top.pri)
TARGET = ut_duiserviceinvoker

INCLUDEPATH += $$DUISRCDIR/service

TEST_SOURCES = \
    $$DUISRCDIR/service/duiserviceinvoker.cpp

# unit test and unit
SOURCES += \
    ut_duiserviceinvoker.cpp \
    $$TEST_SOURCES

# unit test and unit
HEADERS += \
    ut_duiserviceinvoker.h \
    $$DUISRCDIR/service/duiserviceinvoker.h \
    $$DUISRCDIR/service/duiserviceaction.h

include(../common_bot.pri)
