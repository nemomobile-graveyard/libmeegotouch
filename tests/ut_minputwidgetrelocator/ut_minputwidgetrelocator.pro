include(../common_top.pri)
TARGET = ut_minputwidgetrelocator
INCLUDEPATH += \
    $$MSRCDIR/corelib/core

# unit test and unit classes
SOURCES += ut_minputwidgetrelocator.cpp \
           $$MSRCDIR/corelib/core/minputwidgetrelocator.cpp

# unit test and unit classes
HEADERS += \
    ut_minputwidgetrelocator.h \
    mapplicationwindow_p.h \
    mrelocatorstyle.h \
    $$MSRCDIR/corelib/core/minputwidgetrelocator.h

include(../common_bot.pri)
