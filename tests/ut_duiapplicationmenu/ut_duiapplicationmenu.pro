include(../common_top.pri)
TARGET = ut_duiapplicationmenu


INCLUDEPATH += $$DUISRCDIR/corelib/widgets $$DUISRCDIR/corelib/style $$DUISRCDIR/scene

SOURCES += \
    ut_duiapplicationmenu.cpp \

HEADERS += \
    ut_duiapplicationmenu.h

include(../common_bot.pri)
