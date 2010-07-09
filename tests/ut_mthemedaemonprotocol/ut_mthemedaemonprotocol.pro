include(../common_top.pri)

TARGET = ut_mthemedaemonprotocol

INCLUDEPATH += $$MSRCDIR/corelib/theme

# unit test and unit
SOURCES += \
    ut_mthemedaemonprotocol.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.cpp

# unit test and unit
HEADERS += \
    ut_mthemedaemonprotocol.h \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.H

include(../common_bot.pri)
