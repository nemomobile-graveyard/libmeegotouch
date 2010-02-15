include(../common_top.pri)

TARGET = ut_duiapplethandleglesview
INCLUDEPATH += $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/mashup/appletcommunication \
    $$DUISRCDIR/style

QT += network opengl

SOURCES += ut_duiapplethandleglesview.cpp \
           ../stubs/stubbase.cpp

HEADERS += ut_duiapplethandleglesview.h

include(../common_bot.pri)
