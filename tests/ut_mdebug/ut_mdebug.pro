include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/core
TARGET = ut_mdebug

# unit test and unit
SOURCES = \
    $$MSRCDIR/corelib/core/mdebug.cpp \
    ut_mdebug.cpp

# unit test and unit
HEADERS += \
    $$MSRCDIR/corelib/core/mdebug.h \
    ut_mdebug.h


include(../common_bot.pri)
