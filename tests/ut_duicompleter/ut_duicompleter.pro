include(../common_top.pri)

TARGET = ut_duicompleter

SOURCES += \
    ut_duicompleter.cpp \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    $$STUBSDIR/duitheme_stub.h \
    $$STUBSDIR/duiwidgetview_stub.h \
    ut_duicompleter.h \

INCLUDEPATH += $$DUISRCDIR/corelib/widgets/

include(../common_bot.pri)
