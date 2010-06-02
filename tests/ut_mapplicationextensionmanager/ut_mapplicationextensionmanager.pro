include(../common_top.pri)

win32|macx {
    macx {
        QMAKE_LFLAGS -= -F../../lib
        LIBS -= -framework m
    }
    win32:LIBS -= -lmeegotouchcore0 -lmeegotouchviews0
} else {
    LIBS -= -lmeegotouchcore -lmeegotouchviews
}

INCLUDEPATH += \
    $$MSRCDIR/extensions/applicationextension \
    $$MSRCDIR/extensions/mashup/appletcommunication \
    $$MSRCDIR/extensions/mashup/mashup \
    $$MSRCDIR/extensions

# unit test and unit classes
SOURCES += \
    ut_mapplicationextensionmanager.cpp \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionmanager.cpp \
    $$MSRCDIR/extensions/mextensionwatcher.cpp \
    $$MSRCDIR/extensions/mashup/mashup/msubdatastore.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \
    dummy.cpp

# unit test and unit classes
HEADERS += \
    ut_mapplicationextensionmanager.h \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionmanager.h \
    $$MSRCDIR/extensions/mextensionwatcher.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/extensions/mashup/mashup/msubdatastore.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/corelib/widgets/core/mwidget.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.h \
    $$MSRCDIR/corelib/core/mdesktopentry.h \
    $$MSRCDIR/corelib/theme/mtheme.h \
    $$MSRCDIR/corelib/core/mfiledatastore.h \
    $$MSRCDIR/corelib/core/mdatastore.h \
    $$MSRCDIR/corelib/core/mdataaccess.h

include(../common_bot.pri)
