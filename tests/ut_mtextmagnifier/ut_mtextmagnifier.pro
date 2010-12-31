include(../common_top.pri)
TARGET = ut_mtextmagnifier

INCLUDEPATH += $$MSRCDIR

SOURCES += \
    ut_mtextmagnifier.cpp \
    $$MSRCDIR/views/mtextmagnifier.cpp

HEADERS += \
    ut_mtextmagnifier.h \
    mtextmagnifierstyle.h \
    $$MSRCDIR/views/mtextmagnifier.h

include(../common_bot.pri)
