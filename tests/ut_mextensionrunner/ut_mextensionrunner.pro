include(../common_top.pri)
include(../common_mextensions.pri)

QT += network xml

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    CONFIG += link_pkgconfig
    PKGCONFIG += contextsubscriber-1.0
}

SOURCES += ut_mextensionrunner.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_mextensionrunner.h \
    $$MSRCDIR/extensions/applicationextension/mextensionrunner.h

include(../common_bot.pri)
