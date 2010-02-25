include(../common_top.pri)

TARGET = ut_duiappletrunner

INCLUDEPATH += $$DUISRCDIR/applicationextension \
    $$DUISRCDIR/mashup/appletcommunication \
    $$DUISRCDIR/mashup/mashup \
    ../../duiappletrunner

QT += network xml

SOURCES += ut_duiappletrunner.cpp \
    ../../duiappletrunner/duiappletrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiappletrunner.h \
    ../../duiappletrunner/duiappletrunner.h \
    $$DUISRCDIR/applicationextension/duiextensionrunner.h

include(../common_bot.pri)
