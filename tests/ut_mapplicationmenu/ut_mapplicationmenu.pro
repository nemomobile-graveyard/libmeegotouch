include(../common_top.pri)
TARGET = ut_mapplicationmenu


INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style $$MSRCDIR/scene

SOURCES += \
    ut_mapplicationmenu.cpp \

HEADERS += \
    ut_mapplicationmenu.h

include(../common_bot.pri)
