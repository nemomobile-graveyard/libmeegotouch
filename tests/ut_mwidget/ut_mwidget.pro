include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/widgets

TEST_SOURCES +=  \
#    $$MSRCDIR/mwidget.cpp \

SOURCES += \
    ut_mwidget.cpp \
#    $$TEST_SOURCES \
#    $$STUBSDIR/stubbase.cpp \

HEADERS += \
#    $$MSRCDIR/mpannableviewport.h \
#    $$MSRCDIR/mpannablewidget.h \
#    $$MSRCDIR/mwidgetcontroller.h \
#    $$MSRCDIR/mwidget.h \
#    $$STUBSDIR/mpannableviewport_stub.h \
#    $$STUBSDIR/mpannablewidget_stub.h \
#    $$STUBSDIR/mwidgetcontroller_stub.h \
#    $$STUBSDIR/stubbase.h \
    ut_mwidget.h \

include(../common_bot.pri)
