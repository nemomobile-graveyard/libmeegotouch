include(../common_top.pri)
TARGET = ut_mserviceaction

INCLUDEPATH += $$MSRCDIR/corelib/service
INCLUDEPATH += $$MSRCDIR/corelib/core

TEST_SOURCES = \
    $$MSRCDIR/corelib/service/mserviceaction.cpp

# unit test and unit
SOURCES += \
    ut_mserviceaction.cpp \
    $$TEST_SOURCES

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mserviceaction.h \
    $$MSRCDIR/corelib/core/maction.h \
    $$MSRCDIR/corelib/service/mserviceaction.h \
    $$MSRCDIR/corelib/service/mserviceinvoker.h

include(../common_bot.pri)
