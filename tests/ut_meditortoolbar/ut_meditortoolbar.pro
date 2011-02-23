include(../common_top.pri)
TARGET = ut_meditortoolbar

INCLUDEPATH += $$MSRCDIR

SOURCES += \
    ut_meditortoolbar.cpp \
    $$MSRCDIR/views/meditortoolbar.cpp \
    $$MSRCDIR/views/mtopleveloverlay.cpp \
    $$MSRCDIR/views/meditortoolbararrow.cpp

HEADERS += \
    ut_meditortoolbar.h \
    meditortoolbarstyle.h \
    $$MSRCDIR/views/meditortoolbar.h \
    $$MSRCDIR/views/meditortoolbararrow.h \
    $$MSRCDIR/views/mtopleveloverlay.h

include(../common_bot.pri)
