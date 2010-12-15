include(../common_top.pri)

TARGET = ut_mcommonpixmaps

contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}

INCLUDEPATH += $$MSRCDIR/corelib/theme
INCLUDEPATH += $$MSRCDIR/corelib/core

SOURCES += \
    ut_mcommonpixmaps.cpp \
    $$MSRCDIR/corelib/core/mcpumonitor.cpp \
    $$MSRCDIR/corelib/core/mgraphicssystemhelper.cpp \
    $$MSRCDIR/corelib/core/msystemdirectories.cpp \
    $$MSRCDIR/corelib/theme/mcommonpixmaps.cpp \
    $$MSRCDIR/corelib/theme/mimagedirectory.cpp \
    $$MSRCDIR/corelib/theme/mlocalthemedaemon.cpp \
    $$MSRCDIR/corelib/theme/mpixmaphandle.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemon.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemonclient.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.cpp \

HEADERS += \
    ut_mcommonpixmaps.h \
    $$MSRCDIR/corelib/core/mcpumonitor.h \
    $$MSRCDIR/corelib/core/mgraphicssystemhelper.h \
    $$MSRCDIR/corelib/core/msystemdirectories.cpp \
    $$MSRCDIR/corelib/theme/mcommonpixmaps.h \
    $$MSRCDIR/corelib/theme/mimagedirectory.h \
    $$MSRCDIR/corelib/theme/imthemedaemon.h \
    $$MSRCDIR/corelib/theme/mlocalthemedaemon.h \
    $$MSRCDIR/corelib/theme/mpixmaphandle.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemon.h \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.h \
    $$MSRCDIR/corelib/theme/mthemedaemonclient.h \

include(../common_bot.pri)
