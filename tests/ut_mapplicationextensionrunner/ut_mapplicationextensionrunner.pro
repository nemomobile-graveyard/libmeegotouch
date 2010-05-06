include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += ../../mapplicationextensionrunner

QT += network xml

SOURCES += ut_mapplicationextensionrunner.cpp \
    ../../mapplicationextensionrunner/mapplicationextensionrunner.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_mapplicationextensionrunner.h \
    ../../mapplicationextensionrunner/mapplicationextensionrunner.h \
    $$MSRCDIR/applicationextension/mapplicationextensionloader.h \
    $$MSRCDIR/applicationextension/mextensionrunner.h

include(../common_bot.pri)
