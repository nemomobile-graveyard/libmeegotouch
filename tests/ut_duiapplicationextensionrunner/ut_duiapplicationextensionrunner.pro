include(../common_top.pri)
include(../common_duiextensions.pri)

INCLUDEPATH += ../../duiapplicationextensionrunner

QT += network xml

SOURCES += ut_duiapplicationextensionrunner.cpp \
    ../../duiapplicationextensionrunner/duiapplicationextensionrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiapplicationextensionrunner.h \
    ../../duiapplicationextensionrunner/duiapplicationextensionrunner.h \
    $$DUISRCDIR/applicationextension/duiapplicationextensionloader.h \
    $$DUISRCDIR/applicationextension/duiextensionrunner.h

include(../common_bot.pri)
