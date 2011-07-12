include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/core
TARGET = ut_mscenewindowscroller

# unit test and unit
SOURCES = \
    $$MSRCDIR/corelib/core/mabstractscroller.cpp \
    $$MSRCDIR/corelib/core/mscenewindowscroller.cpp \
    ut_mscenewindowscroller.cpp

# unit test and unit
HEADERS += \
    $$MSRCDIR/corelib/core/mabstractscroller.h \
    $$MSRCDIR/corelib/core/mscenewindowscroller.h \
    ut_mscenewindowscroller.h


include(../common_bot.pri)
