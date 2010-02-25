include(../common_top.pri)
TARGET = ut_duiappletinstancemanager
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface $$DUISRCDIR/mashup/mashup $$DUISRCDIR/events $$DUISRCDIR/applicationextension

QT += core network gui svg dbus

# unit test and unit classes
SOURCES += \
    ut_duiappletinstancemanager.cpp \
    $$DUISRCDIR/mashup/mashup/duiappletinstancemanager.cpp \
    $$DUISRCDIR/mashup/mashup/duiappletinstancedata.cpp \
    $$DUISRCDIR/mashup/mashup/duiappletid.cpp \
    ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_duiappletinstancemanager.h \
    $$DUISRCDIR/mashup/mashup/duiappletinstancemanager.h \
    $$DUISRCDIR/mashup/mashup/duiappletinstancedata.h \
    $$DUISRCDIR/mashup/mashup/duiappletid.h \
    $$DUISRCDIR/mashup/mashup/duiapplethandle.h \
    $$DUISRCDIR/mashup/mashup/duiapplethandlemodel.h \
    $$DUISRCDIR/mashup/mashup/duiappletinstancemanagerdbusadaptor.h \
    $$DUISRCDIR/applicationextension/duiextensionhandle.h \
    $$DUISRCDIR/applicationextension/duiextensionhandlemodel.h \
    $$DUISRCDIR/events/duiorientationtracker.h

include(../common_bot.pri)
