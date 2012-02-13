include(../common_top.pri)

TARGET = ut_mwindow

INCLUDEPATH += $$MSRCDIR/corelib/events

LIBS += -lX11

# Input
HEADERS += \
    ut_mwindow.h \

SOURCES += \
    ut_mwindow.cpp \

HEADERS += \

include(../common_bot.pri)
