include(../common_top.pri)

TARGET = ut_mapplicationwindow

INCLUDEPATH += \
    $$MSRCDIR/corelib/core \
    $$MSRCDIR/corelib/scene \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    CONFIG += link_pkgconfig
    PKGCONFIG += contextsubscriber-1.0
}

SOURCES += \
    ut_mapplicationwindow.cpp

HEADERS += \
    ut_mapplicationwindow.h

support_files.files += \
    ut_mapplicationwindow.css

include(../common_bot.pri)
