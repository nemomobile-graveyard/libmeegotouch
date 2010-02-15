include(../common_top.pri)
TARGET = ut_duiserviceaction

INCLUDEPATH += $$DUISRCDIR/service
INCLUDEPATH += $$DUISRCDIR/core

TEST_SOURCES = \
    $$DUISRCDIR/service/duiserviceaction.cpp

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
    $$DUISRCDIR/core/duiaction.h \
    $$DUISRCDIR/service/duiserviceaction.h \
    $$DUISRCDIR/service/duiserviceinvoker.h

include(../common_bot.pri)
