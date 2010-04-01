include(../common_top.pri)
INCLUDEPATH += $$DUISRCDIR/corelib/core
TARGET = ut_duiapplicationservice

# unit test and unit
SOURCES = \
    ut_duiapplicationservice.cpp \
    $$DUISRCDIR/corelib/core/duiapplicationservice.cpp \

# unit test and unit
HEADERS += \
    ut_duiapplicationservice.h 


include(../common_bot.pri)
