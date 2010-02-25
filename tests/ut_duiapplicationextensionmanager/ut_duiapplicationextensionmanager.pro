include(../common_top.pri)

win32|macx {
    macx {
        QMAKE_LFLAGS -= -F../../lib
        LIBS -= -framework dui
    }
    win32:LIBS -= -L../../lib -ldui0
} else {
    LIBS -= ../../lib/libdui.so
}

TARGET = ut_duiapplicationextensionmanager
INCLUDEPATH += \
    $$DUISRCDIR/applicationextension \
    $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiapplicationextensionmanager.cpp \
    $$DUISRCDIR/applicationextension/duiapplicationextensionmanager.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiapplicationextensionmanager.h \
    $$DUISRCDIR/applicationextension/duiapplicationextensionmanager.h \
    $$DUISRCDIR/applicationextension/duiextensionhandle.h \
    $$DUISRCDIR/applicationextension/duiextensionhandlemodel.h \
    $$DUISRCDIR/widgets/duiwidgetmodel.h \
    $$DUISRCDIR/widgets/core/duiwidget.h \
    $$DUISRCDIR/widgets/core/duiwidgetcontroller.h \
    $$DUISRCDIR/core/duidesktopentry.h \
    $$DUISRCDIR/theme/duitheme.h \
    $$DUISRCDIR/mashup/mashup/duifiledatastore.h \
    $$DUISRCDIR/mashup/mashup/duidatastore.h \
    $$DUISRCDIR/mashup/mashup/duidataaccess.h

include(../common_bot.pri)
