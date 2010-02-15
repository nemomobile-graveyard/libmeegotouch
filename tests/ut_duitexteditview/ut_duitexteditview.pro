include(../common_top.pri)
TARGET = ut_duitexteditview

TEST_SOURCES = \

SOURCES += \
    ut_duitexteditview.cpp \
    $$TEST_SOURCES \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_duitexteditview.h \
#    $$STUBSDIR/duistyledescription_stub.h \
#    $$STUBSDIR/duistyle_stub.h \
#    $$STUBSDIR/duitheme_stub.h \
#    $$STUBSDIR/duiwidgetcontroller_stub.h \
#    $$STUBSDIR/duiwidget_stub.h \

include(../common_bot.pri)
