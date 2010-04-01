include(../common_top.pri)

TARGET = ut_duisettings

TEST_SOURCES = \
#    $$DUISRCDIR/settings.cpp \

# unit test and unit
SOURCES += \
    ut_duisettings.cpp \
#    $$TEST_SOURCES  \

# base classes
SOURCES += \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettings.h \
#    $$DUISRCDIR/settings.h \

# base classes
HEADERS += \

# service classes
HEADERS += \

include(../common_bot.pri)
