include(../common_top.pri)

LIBS -= ../../lib/libdui.so

TARGET = ut_duidesktopentry

HEADERS += ut_duidesktopentry.h \
    $$DUISRCDIR/core/duidesktopentry.h \
    $$DUISRCDIR/core/duidesktopentry_p.h \
    $$DUISRCDIR/i18n/duilocale.h \
    $$STUBSDIR/stubbase.h

SOURCES += ut_duidesktopentry.cpp \
    $$DUISRCDIR/core/duidesktopentry.cpp \
    $$STUBSDIR/stubbase.cpp


INCLUDEPATH += $$DUISRCDIR/core

include(../common_bot.pri)
