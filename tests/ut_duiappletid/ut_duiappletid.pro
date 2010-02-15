include(../common_top.pri)

TARGET = ut_duiappletid
INCLUDEPATH += $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/mashup/appletcommunication

QT += network

SOURCES += ut_duiappletid.cpp \
    $$DUISRCDIR/mashup/mashup/duiappletid.cpp

HEADERS += ut_duiappletid.h \
    $$DUISRCDIR/mashup/mashup/duiappletid.h

include(../common_bot.pri)
