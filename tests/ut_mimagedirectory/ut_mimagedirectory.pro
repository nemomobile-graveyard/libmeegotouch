include(../common_top.pri)

contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}

INCLUDEPATH += \
    $${M_SOURCE_TREE}/src/corelib/theme \
    $${M_SOURCE_TREE}/src/corelib/core

SOURCES += \
    ut_mimagedirectory.cpp \
    $${M_SOURCE_TREE}/src/corelib/theme/mimagedirectory.cpp \
    $${M_SOURCE_TREE}/src/corelib/theme/mpixmaphandle.cpp \
    $${M_SOURCE_TREE}/src/corelib/core/mgraphicssystemhelper.cpp \
    $${M_SOURCE_TREE}/src/corelib/core/msystemdirectories.cpp \

linux-g++-maemo {
    SOURCES += \
        $${M_SOURCE_TREE}/src/corelib/theme/inotifywrapper/fstreenode.cpp \
        $${M_SOURCE_TREE}/src/corelib/theme/inotifywrapper/utils.cpp \
        $${M_SOURCE_TREE}/src/corelib/theme/inotifywrapper/inotify.cpp \
        $${M_SOURCE_TREE}/src/corelib/theme/inotifywrapper/inotify.watch.cpp
}

HEADERS += \
    ut_mimagedirectory.h \
    $${M_SOURCE_TREE}/src/corelib/theme/mimagedirectory.h \
    $${M_SOURCE_TREE}/src/corelib/theme/mpixmaphandle.h \
    $${M_SOURCE_TREE}/src/corelib/core/mgraphicssystemhelper.h \

linux-g++-maemo {
    HEADERS += \
        $${M_SOURCE_TREE}/src/corelib/theme/inotifywrapper/fstreenode.h \
        $${M_SOURCE_TREE}/src/corelib/theme/inotifywrapper/utils.h \
        $${M_SOURCE_TREE}/src/corelib/theme/inotifywrapper/inotify.h \
        $${M_SOURCE_TREE}/src/corelib/theme/inotifywrapper/inotify.watch.h
}

locale_files.files += ut_mimagedirectory-samples/*
locale_files.path = $$[QT_INSTALL_LIBS]/libmeegotouch-tests/ut_mimagedirectory-samples
INSTALLS += locale_files

support_files.files += ut_mimagedirectory-image-svg.svg

include(../common_bot.pri)
