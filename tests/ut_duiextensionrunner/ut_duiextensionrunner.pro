include(../common_top.pri)
include(../common_duiextensions.pri)

QT += network xml

SOURCES += ut_duiextensionrunner.cpp \
    $$DUISRCDIR/extensions/applicationextension/duiextensionrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiextensionrunner.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionrunner.h

include(../common_bot.pri)
