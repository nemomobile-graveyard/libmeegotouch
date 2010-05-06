include(../common_top.pri)
TARGET = ut_mkutclassname

TEST_SOURCES = \
    $$MSRCDIR/mkutclassname.cpp \

# unit test and unit
SOURCES += \
    ut_mkutclassname.cpp \
    $$TEST_SOURCES \ 

# base classes
SOURCES += \
    mkutbasesources

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mkutclassname.h \
    $$MSRCDIR/mkutclassname.h \

# base classes
HEADERS += \
    mkutbaseheaders

# service classes
HEADERS += \
    mkutserviceheaders

include(../common_bot.pri)
