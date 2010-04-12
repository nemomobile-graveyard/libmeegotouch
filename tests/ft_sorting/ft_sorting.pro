include(../common_top.pri)

TARGET = ft_sorting

TEST_SOURCES = \
#    $$MSRCDIR/mlocale.cpp

# Input
HEADERS += \
    ft_sorting.h \

SOURCES += \
    ft_sorting.cpp \
    $$TEST_SOURCES \

# service classes
SOURCES += \
#    $$STUBSDIR/stubbase.cpp \


include(../common_bot.pri)
