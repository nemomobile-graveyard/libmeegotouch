include(../common_top.pri)
TARGET = ut_minputmethodstate

# unit test and unit classes
SOURCES += \
    ut_minputmethodstate.cpp \
    $$MSRCDIR/corelib/core/minputmethodstate.cpp

# unit test and unit classes
HEADERS += \
   ut_minputmethodstate.h \
   $$MSRCDIR/corelib/core/minputmethodstate.h \
   $$MSRCDIR/corelib/core/minputmethodstate_p.h

include(../common_bot.pri)
