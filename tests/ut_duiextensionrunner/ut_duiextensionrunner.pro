include(../common_top.pri)

TARGET = ut_duiextensionrunner

INCLUDEPATH += $$DUISRCDIR/applicationextension \
    $$DUISRCDIR/mashup/appletcommunication \
    $$DUISRCDIR/mashup/mashup

QT += network xml

SOURCES += ut_duiextensionrunner.cpp \
    $$DUISRCDIR/applicationextension/duiextensionrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiextensionrunner.h \
    $$DUISRCDIR/applicationextension/duiextensionrunner.h

include(../common_bot.pri)
