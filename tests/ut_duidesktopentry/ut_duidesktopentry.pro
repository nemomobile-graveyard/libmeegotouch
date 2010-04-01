include(../common_top.pri)

#LIBS -= -lduicore
#LIBS -= -lduiviews

TARGET = ut_duidesktopentry

HEADERS += ut_duidesktopentry.h \
    $$DUISRCDIR/corelib/core/duidesktopentry.h \
    $$DUISRCDIR/corelib/core/duidesktopentry_p.h \
    $$DUISRCDIR/i18n/duilocale.h \
    $$STUBSDIR/stubbase.h

SOURCES += ut_duidesktopentry.cpp \
    $$DUISRCDIR/corelib/core/duidesktopentry.cpp \
    $$STUBSDIR/stubbase.cpp


INCLUDEPATH += $$DUISRCDIR/corelib/core

include(../common_bot.pri)
