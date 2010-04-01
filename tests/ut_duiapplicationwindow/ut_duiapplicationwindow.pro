include(../common_top.pri)

TARGET = ut_duiapplicationwindow
INCLUDEPATH += $$DUISRCDIR/corelib/core $$DUISRCDIR/scene $$DUISRCDIR/corelib/widgets

SOURCES += \
    ut_duiapplicationwindow.cpp \

HEADERS += \
    ut_duiapplicationwindow.h

include(../common_bot.pri)
