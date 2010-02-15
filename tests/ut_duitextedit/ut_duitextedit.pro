include(../common_top.pri)

TARGET = ut_duitextedit

SOURCES += \
    ut_duitextedit.cpp \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    $$STUBSDIR/duipannableviewport_stub.h \
    $$STUBSDIR/duipannablewidget_stub.h \
    $$STUBSDIR/duitexteditview_stub.h \
    $$STUBSDIR/duitheme_stub.h \
    $$STUBSDIR/duiwidgetview_stub.h \
    ut_duitextedit.h \

INCLUDEPATH += $$DUISRCDIR/widgets/

include(../common_bot.pri)
