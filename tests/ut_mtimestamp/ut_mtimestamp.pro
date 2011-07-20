include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/core
TARGET = ut_mtimestamp

# unit test and unit
SOURCES = \
    $$MSRCDIR/corelib/core/mtimestamp.cpp \
    ut_mtimestamp.cpp

# unit test and unit
HEADERS += \
    $$MSRCDIR/corelib/core/mtimestamp.h \
    ut_mtimestamp.h


include(../common_bot.pri)
