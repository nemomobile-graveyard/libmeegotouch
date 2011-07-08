include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/core
TARGET = ut_mdbusservicewaiter

# unit test and unit
SOURCES = \
    $$MSRCDIR/corelib/core/mdbusservicewaiter.cpp \
    ut_mdbusservicewaiter.cpp

# unit test and unit
HEADERS += \
    $$MSRCDIR/corelib/core/mdbusservicewaiter_p.h \
    ut_mdbusservicewaiter.h


include(../common_bot.pri)
