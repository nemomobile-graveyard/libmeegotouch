include(../common_top.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/scene \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style \
    $$MSRCDIR/corelib/events

TARGET = ut_mscenemanager

# Input
HEADERS += \
    ut_mscenemanager.h \

SOURCES += \
    ut_mscenemanager.cpp \

include(../common_bot.pri)
