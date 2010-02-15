include(../common_top.pri)

TARGET = ut_duiappletrunner

APPLETRUNNERDIR = ../../duiappletrunner
INCLUDEPATH += $$APPLETRUNNERDIR \
    $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/mashup/appletinterface \
    $$DUISRCDIR/mashup/appletcommunication

QT += network xml

SOURCES += ut_duiappletrunner.cpp \
    $$APPLETRUNNERDIR/duiappletrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiappletrunner.h \
    $$APPLETRUNNERDIR/duiappletrunner.h

include(../common_bot.pri)
