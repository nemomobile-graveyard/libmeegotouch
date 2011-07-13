include(../common_top.pri)

TARGET = ut_mdesktopentry

HEADERS += ut_mdesktopentry.h \
    $$MSRCDIR/corelib/core/mdesktopentry.h \
    $$MSRCDIR/corelib/core/mdesktopentry_p.h \
    $$MSRCDIR/corelib/i18n/mlocale.h \
    $$STUBSDIR/stubbase.h

SOURCES += $$MSRCDIR/corelib/core/mdesktopentry.cpp \
    ut_mdesktopentry.cpp \
    $$STUBSDIR/stubbase.cpp


INCLUDEPATH += $$MSRCDIR/corelib/core

include(../common_bot.pri)
