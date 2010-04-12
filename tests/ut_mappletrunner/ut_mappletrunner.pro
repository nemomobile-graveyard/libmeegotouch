include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    ../../mappletrunner

QT += network xml

SOURCES += ut_mappletrunner.cpp \
    ../../mappletrunner/mappletrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_mappletrunner.h \
    ../../mappletrunner/mappletrunner.h \
    $$MSRCDIR/extensions/applicationextension/mextensionrunner.h

include(../common_bot.pri)
