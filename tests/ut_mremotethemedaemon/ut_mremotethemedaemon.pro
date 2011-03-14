include(../common_top.pri)

contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}

TARGET = ut_mremotethemedaemon

INCLUDEPATH += $$MSRCDIR/corelib/theme
INCLUDEPATH += $$MSRCDIR/corelib/core

SOURCES += \
    ut_mremotethemedaemon.cpp \
    $$MSRCDIR/corelib/theme/mpixmaphandle.cpp \
    $$MSRCDIR/corelib/theme/mremotethemedaemon.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.cpp \

HEADERS += \
    ut_mremotethemedaemon.h \
    $$MSRCDIR/corelib/theme/imthemedaemon.h \
    $$MSRCDIR/corelib/theme/mpixmaphandle.h \
    $$MSRCDIR/corelib/theme/mremotethemedaemon.h \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.h \

support_files.files += \
    priorities.conf

include(../common_bot.pri)
