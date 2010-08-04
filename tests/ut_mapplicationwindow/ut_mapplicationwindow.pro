include(../common_top.pri)

TARGET = ut_mapplicationwindow

INCLUDEPATH += \
    $$MSRCDIR/corelib/core \
    $$MSRCDIR/corelib/scene \
    $$MSRCDIR/corelib/widgets

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    CONFIG += link_pkgconfig
    PKGCONFIG += contextsubscriber-1.0
}

SOURCES += \
    ut_mapplicationwindow.cpp

HEADERS += \
    ut_mapplicationwindow.h

include(../common_bot.pri)
