include(../common_top.pri)

TARGET = ut_msettings

TEST_SOURCES = \
#    $$MSRCDIR/settings.cpp \

# unit test and unit
SOURCES += \
    ut_msettings.cpp \
#    $$TEST_SOURCES  \

# base classes
SOURCES += \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_msettings.h \
#    $$MSRCDIR/settings.h \

# base classes
HEADERS += \

# service classes
HEADERS += \

include(../common_bot.pri)
