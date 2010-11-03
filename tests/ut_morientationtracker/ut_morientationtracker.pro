include(../common_top.pri)
include(../morientationtrackertestbridge/morientationtrackertestbridge.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/events \

TARGET = ut_morientationtracker

# Input
HEADERS += \
    ut_morientationtracker.h \

SOURCES += \
    ut_morientationtracker.cpp \

include(../common_bot.pri)
