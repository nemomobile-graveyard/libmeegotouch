include(../common_top.pri)
INCLUDEPATH += \
    $$DUISRCDIR/corelib/scene \
    $$DUISRCDIR/corelib/widgets \
    $$DUISRCDIR/corelib/style \
    $$DUISRCDIR/corelib/events

TARGET = ut_duiscenemanager

# Input
HEADERS += \
    ut_duiscenemanager.h \

SOURCES += \
    ut_duiscenemanager.cpp \

include(../common_bot.pri)
