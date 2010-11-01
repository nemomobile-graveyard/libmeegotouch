include(../common_top.pri)
TARGET = ut_mscrollchain
INCLUDEPATH += $$MSRCDIR/corelib/core

# unit test and unit classes
SOURCES += ut_mscrollchain.cpp \
           $$MSRCDIR/corelib/core/mscrollchain.cpp \
           $$MSRCDIR/corelib/core/mabstractscroller.cpp

# unit test and unit classes
HEADERS += ut_mscrollchain.h \
           $$MSRCDIR/corelib/core/mscrollchain.h \
           $$MSRCDIR/corelib/core/mabstractscroller.h

include(../common_bot.pri)
