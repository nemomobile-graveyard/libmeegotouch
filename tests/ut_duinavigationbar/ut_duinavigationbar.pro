include(../common_top.pri)

TARGET = ut_duinavigationbar

INCLUDEPATH += $$DUISRCDIR/corelib/widgets $$DUISRCDIR/corelib/style $$DUISRCDIR/scene

SOURCES += \
    ut_duinavigationbar.cpp \

HEADERS += \
    ut_duinavigationbar.h

include(../common_bot.pri)
