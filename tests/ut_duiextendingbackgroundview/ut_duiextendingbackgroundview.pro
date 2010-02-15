include(../common_top.pri)
TARGET = ut_duiextendingbackgroundview

TEST_SOURCES = \

# unit test and unit
SOURCES += \
    ut_duiextendingbackgroundview.cpp \
    $$TEST_SOURCES \ 

# base classes
SOURCES += \


# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_duiextendingbackgroundview.h

include(../common_bot.pri)
