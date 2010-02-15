include(../common_top.pri)
TARGET = ut_duiactionprovider
INCLUDEPATH += $$DUISRCDIR/service

TEST_SOURCES = \
    $$DUISRCDIR/service/duiactionprovider.cpp

# unit test and unit
SOURCES += \
    ut_duiactionprovider.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$TEST_SOURCES

# unit test and unit
HEADERS += \
    ut_duiactionprovider.h \
    $$DUISRCDIR/service/duiactionprovider.h

# service classes
HEADERS += \
    $$DUISRCDIR/core/duiaction.h \
    $$DUISRCDIR/service/duiserviceaction.h

include(../common_bot.pri)
