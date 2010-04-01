include(../common_top.pri)
include(../common_duiextensions.pri)

INCLUDEPATH += \
    $$DUISRCDIR/events \

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_duiappletinstancemanager.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinstancemanager.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinstancedata.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletid.cpp \
    dummy.cpp \
    ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_duiappletinstancemanager.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinstancemanager.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinstancedata.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletid.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiapplethandle.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiapplethandlemodel.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletinstancemanagerdbusadaptor.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandle.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionhandlemodel.h \
    $$DUISRCDIR/events/duiorientationtracker.h

include(../common_bot.pri)
