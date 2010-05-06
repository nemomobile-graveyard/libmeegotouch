include(../common_top.pri)
TARGET = ut_mtexteditview

TEST_SOURCES = \

SOURCES += \
    ut_mtexteditview.cpp \
    $$TEST_SOURCES \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_mtexteditview.h \
#    $$STUBSDIR/mstyledescription_stub.h \
#    $$STUBSDIR/mstyle_stub.h \
#    $$STUBSDIR/mtheme_stub.h \
#    $$STUBSDIR/mwidgetcontroller_stub.h \
#    $$STUBSDIR/mwidget_stub.h \

include(../common_bot.pri)
