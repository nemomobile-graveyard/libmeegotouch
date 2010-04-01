include(../common_top.pri)

win32|macx {
    macx {
        QMAKE_LFLAGS -= -F../../lib
        LIBS -= -framework dui
    }
    win32:LIBS -= -lduicore0 -lduiviews0
} else {
    LIBS -= -lduicore -lduiviews
}

INCLUDEPATH += \
    $$DUISRCDIR/extensions/applicationextension \
    $$DUISRCDIR/extensions/mashup/appletcommunication \
    $$DUISRCDIR/extensions/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiapplicationextensionmanager.cpp \
    $$DUISRCDIR/extensions/applicationextension/duiapplicationextensionmanager.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \
    dummy.cpp

# unit test and unit classes
HEADERS += \
    ut_duiapplicationextensionmanager.h \
    $$DUISRCDIR/extensions/applicationextension/duiapplicationextensionmanager.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandle.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandlemodel.h \
    $$DUISRCDIR/corelib/widgets/duiwidgetmodel.h \
    $$DUISRCDIR/corelib/widgets/core/duiwidget.h \
    $$DUISRCDIR/corelib/widgets/core/duiwidgetcontroller.h \
    $$DUISRCDIR/corelib/core/duidesktopentry.h \
    $$DUISRCDIR/corelib/theme/duitheme.h \
    $$DUISRCDIR/corelib/core/duifiledatastore.h \
    $$DUISRCDIR/corelib/core/duidatastore.h \
    $$DUISRCDIR/corelib/core/duidataaccess.h

include(../common_bot.pri)
