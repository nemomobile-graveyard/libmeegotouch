include(../common_top.pri)
include(../common_duiextensions.pri)

INCLUDEPATH += \
    ../../duiappletrunner

QT += network xml

SOURCES += ut_duiappletrunner.cpp \
    ../../duiappletrunner/duiappletrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_duiappletrunner.h \
    ../../duiappletrunner/duiappletrunner.h \
    $$DUISRCDIR/extensions/applicationextension/duiextensionrunner.h

include(../common_bot.pri)
