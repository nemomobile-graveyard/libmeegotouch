include(../common_top.pri)

TARGET = ut_mremotethemedaemon

INCLUDEPATH += $$MSRCDIR/corelib/theme
INCLUDEPATH += $$MSRCDIR/corelib/core

SOURCES += \
    ut_mremotethemedaemon.cpp \
    $$MSRCDIR/corelib/theme/mremotethemedaemon.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.cpp \

HEADERS += \
    ut_mremotethemedaemon.h \
    $$MSRCDIR/corelib/theme/imthemedaemon.h \
    $$MSRCDIR/corelib/theme/mremotethemedaemon.h \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.h \

include(../common_bot.pri)
