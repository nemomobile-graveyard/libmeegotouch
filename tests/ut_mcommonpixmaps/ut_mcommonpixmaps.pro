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
    $$MSRCDIR/corelib/theme/mpixmaphandle.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemon.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemonclient.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.cpp \

linux-g++-maemo {
    SOURCES += \
        $$MSRCDIR/corelib/theme/inotifywrapper/fstreenode.cpp \
        $$MSRCDIR/corelib/theme/inotifywrapper/utils.cpp \
        $$MSRCDIR/corelib/theme/inotifywrapper/inotify.cpp \
        $$MSRCDIR/corelib/theme/inotifywrapper/inotify.watch.cpp
}

HEADERS += \
    ut_mcommonpixmaps.h \
    $$MSRCDIR/corelib/core/mcpumonitor.h \
    $$MSRCDIR/corelib/core/mgraphicssystemhelper.h \
    $$MSRCDIR/corelib/theme/mcommonpixmaps.h \
    $$MSRCDIR/corelib/theme/mimagedirectory.h \
    $$MSRCDIR/corelib/theme/mpixmaphandle.cpp \
    $$MSRCDIR/corelib/theme/mthemedaemon.h \
    $$MSRCDIR/corelib/theme/mthemedaemonprotocol.h \
    $$MSRCDIR/corelib/theme/mthemedaemonclient.h \

linux-g++-maemo {
    HEADERS += \
        $$MSRCDIR/corelib/theme/inotifywrapper/fstreenode.h \
        $$MSRCDIR/corelib/theme/inotifywrapper/utils.h \
        $$MSRCDIR/corelib/theme/inotifywrapper/inotify.h \
        $$MSRCDIR/corelib/theme/inotifywrapper/inotify.watch.h
}

support_files.files += svg

include(../common_bot.pri)
