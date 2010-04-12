include(../common_top.pri)

TARGET = ut_mappletid
INCLUDEPATH += $$MSRCDIR/extensions/mashup/mashup \
    $$MSRCDIR/extensions/mashup/appletcommunication

QT += network

SOURCES += ut_mappletid.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.cpp

HEADERS += ut_mappletid.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.h

include(../common_bot.pri)
