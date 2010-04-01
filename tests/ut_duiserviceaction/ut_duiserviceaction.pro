include(../common_top.pri)
TARGET = ut_duiserviceaction

INCLUDEPATH += $$DUISRCDIR/corelib/service
INCLUDEPATH += $$DUISRCDIR/corelib/core

TEST_SOURCES = \
    $$DUISRCDIR/corelib/service/duiserviceaction.cpp

# unit test and unit
SOURCES += \
    ut_duiserviceaction.cpp \
    $$TEST_SOURCES

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duiserviceaction.h \
    $$DUISRCDIR/corelib/core/duiaction.h \
    $$DUISRCDIR/corelib/service/duiserviceaction.h \
    $$DUISRCDIR/corelib/service/duiserviceinvoker.h

include(../common_bot.pri)
