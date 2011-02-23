include(../common_top.pri)
TARGET = ut_mtextmagnifier

INCLUDEPATH += $$MSRCDIR

SOURCES += \
    ut_mtextmagnifier.cpp \
    $$MSRCDIR/views/mtextmagnifier.cpp \
    $$MSRCDIR/views/mtopleveloverlay.cpp

HEADERS += \
    ut_mtextmagnifier.h \
    mtextmagnifierstyle.h \
    $$MSRCDIR/views/mtextmagnifier.h \
    $$MSRCDIR/views/mtopleveloverlay.h

include(../common_bot.pri)
