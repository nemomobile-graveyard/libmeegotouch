include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/core
TARGET = ut_msyslogclient

# unit test and unit
SOURCES = \
    $$MSRCDIR/corelib/core/msyslogclient.cpp \
    ut_msyslogclient.cpp

# unit test and unit
HEADERS += \
    $$MSRCDIR/corelib/core/msyslogclient.h \
    ut_msyslogclient.h


include(../common_bot.pri)
