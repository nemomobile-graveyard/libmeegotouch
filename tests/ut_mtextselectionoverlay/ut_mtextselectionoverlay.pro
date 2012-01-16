include(../common_top.pri)
TARGET = ut_mtextselectionoverlay

INCLUDEPATH += $$MSRCDIR

SOURCES += \
    ut_mtextselectionoverlay.cpp \
    $$MSRCDIR/views/mtextselectionoverlay.cpp \
    $$MSRCDIR/views/mtextselectionhandle.cpp \
    $$MSRCDIR/views/meatgesturefilter.cpp \
    $$MSRCDIR/views/mtopleveloverlay.cpp

HEADERS += \
    ut_mtextselectionoverlay.h \
    mtextselectionhandlestyle.h \
    testview.h \
    $$MSRCDIR/views/mtextselectionoverlay.h \
    $$MSRCDIR/views/mtextselectionhandle.h \
    $$MSRCDIR/views/meatgesturefilter.h \
    $$MSRCDIR/views/mtopleveloverlay.h

include(../common_bot.pri)
