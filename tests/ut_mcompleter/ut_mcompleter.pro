include(../common_top.pri)

TARGET = ut_mcompleter

SOURCES += \
    ut_mcompleter.cpp \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    $$STUBSDIR/mtheme_stub.h \
    $$STUBSDIR/mwidgetview_stub.h \
    ut_mcompleter.h \

INCLUDEPATH += $$MSRCDIR/corelib/widgets/

include(../common_bot.pri)
