include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/events \

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_mappletinstancemanager.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancemanager.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancedata.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.cpp \
    dummy.cpp \
    ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_mappletinstancemanager.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancemanager.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancedata.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandle.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandlemodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancemanagerdbusadaptor.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/events/morientationtracker.h

include(../common_bot.pri)
