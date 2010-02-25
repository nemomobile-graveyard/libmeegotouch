include(../common_top.pri)

TARGET = ut_duiapplicationextensionrunner

INCLUDEPATH += $$DUISRCDIR/applicationextension \
    $$DUISRCDIR/mashup/appletcommunication \
    $$DUISRCDIR/mashup/mashup \
    ../../duiapplicationextensionrunner

QT += network xml

SOURCES += ut_duiapplicationextensionrunner.cpp \
    ../../duiapplicationextensionrunner/duiapplicationextensionrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiapplicationextensionrunner.h \
    ../../duiapplicationextensionrunner/duiapplicationextensionrunner.h \
    $$DUISRCDIR/applicationextension/duiapplicationextensionloader.h \
    $$DUISRCDIR/applicationextension/duiextensionrunner.h

include(../common_bot.pri)
