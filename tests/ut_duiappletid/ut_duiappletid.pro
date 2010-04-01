include(../common_top.pri)

TARGET = ut_duiappletid
INCLUDEPATH += $$DUISRCDIR/extensions/mashup/mashup \
    $$DUISRCDIR/extensions/mashup/appletcommunication

QT += network

SOURCES += ut_duiappletid.cpp \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletid.cpp

HEADERS += ut_duiappletid.h \
    $$DUISRCDIR/extensions/mashup/mashup/duiappletid.h

include(../common_bot.pri)
