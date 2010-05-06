include(../common_top.pri)
TARGET = ut_mextendingbackgroundview

TEST_SOURCES = \

# unit test and unit
SOURCES += \
    ut_mextendingbackgroundview.cpp \
    $$TEST_SOURCES \ 

# base classes
SOURCES += \


# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_mextendingbackgroundview.h

include(../common_bot.pri)
