include(../common_top.pri)
TARGET = ut_minputwidgetrelocator
INCLUDEPATH += \
    $$MSRCDIR/corelib/core \
    ../ut_mscrollchain

# unit test and unit classes
SOURCES += \
    ut_minputwidgetrelocator.cpp \
    $$MSRCDIR/corelib/core/minputwidgetrelocator.cpp \
    $$MSRCDIR/corelib/core/mscrollchain.cpp \
    $$MSRCDIR/corelib/core/mabstractscroller.cpp \
    ../ut_mscrollchain/scroller.cpp \
    ../ut_mscrollchain/scrollablewidget.cpp

# unit test and unit classes
HEADERS += \
    ut_minputwidgetrelocator.h \
    mapplicationwindow_p.h \
    mrelocatorstyle.h \
    mscenemanager.h \
    mscene.h \
    $$MSRCDIR/corelib/core/minputwidgetrelocator.h \
    $$MSRCDIR/corelib/core/mscrollchain.h \
    $$MSRCDIR/corelib/core/mabstractscroller.h \
   ../ut_mscrollchain/scroller.h \
   ../ut_mscrollchain/scrollablewidget.h

CXXFLAGS += -include mscenemanager.h \
            -include mscene.h

include(../common_bot.pri)
