include(../common_top.pri)
TARGET = ut_duiinputmethodstate

# unit test and unit classes
SOURCES += \
    ut_duiinputmethodstate.cpp \
    $$DUISRCDIR/core/duiinputmethodstate.cpp

# unit test and unit classes
HEADERS += \
   ut_duiinputmethodstate.h \
   $$DUISRCDIR/core/duiinputmethodstate.h \
   $$DUISRCDIR/core/duiinputmethodstate_p.h

include(../common_bot.pri)
