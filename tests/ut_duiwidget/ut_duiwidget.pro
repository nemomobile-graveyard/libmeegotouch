include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/corelib/widgets

TEST_SOURCES +=  \
#    $$DUISRCDIR/duiwidget.cpp \

SOURCES += \
    ut_duiwidget.cpp \
#    $$TEST_SOURCES \
#    $$STUBSDIR/stubbase.cpp \

HEADERS += \
#    $$DUISRCDIR/duipannableviewport.h \
#    $$DUISRCDIR/duipannablewidget.h \
#    $$DUISRCDIR/duiwidgetcontroller.h \
#    $$DUISRCDIR/duiwidget.h \
#    $$STUBSDIR/duipannableviewport_stub.h \
#    $$STUBSDIR/duipannablewidget_stub.h \
#    $$STUBSDIR/duiwidgetcontroller_stub.h \
#    $$STUBSDIR/stubbase.h \
    ut_duiwidget.h \

include(../common_bot.pri)
