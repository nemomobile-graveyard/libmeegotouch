include(../common_top.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/events \

TARGET = ut_morientationtracker

# Input
HEADERS += \
    ut_morientationtracker.h \
    ../../src/corelib/events/mservicelistener.h

SOURCES += \
    ut_morientationtracker.cpp \
    ../stubs/stubbase.cpp

include(../common_bot.pri)
