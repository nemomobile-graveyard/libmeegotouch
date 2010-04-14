include(../common_top.pri)

TARGET = ut_mstatusbar

INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/scene $$MSRCDIR/corelib/events $$MSRCDIR/corelib/style $$MSRCDIR/corelib/animation/scene $$MSRCDIR/corelib/animation $$MSRCDIR/corelib/animation/widget

SOURCES += \
    ut_mstatusbar.cpp \
    $$MSRCDIR/corelib/widgets/mstatusbar.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += \
    ut_mstatusbar.h \
    $$MSRCDIR/corelib/widgets/mstatusbar.h \
    $$MSRCDIR/corelib/widgets/mstatusbar_p.h \
    $$STUBSDIR/stubbase.h

include(../common_bot.pri)
