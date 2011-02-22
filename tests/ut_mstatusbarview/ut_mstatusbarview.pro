include(../common_top.pri)
TARGET = ut_mstatusbarview

INCLUDEPATH += $$MSRCDIR/views $$STUBSDIR $$MSRCDIR/corelib/widgets

TEST_SOURCES = \
    $$MSRCDIR/views/mstatusbarview.cpp \

# unit test and unit
SOURCES += \
    ut_mstatusbarview.cpp \
    mstatusbarviewdbuswrapper.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mstatusbarview.h \
    mstatusbarviewdbuswrapper.h \
    $$MSRCDIR/views/mstatusbarview.h \

include(../common_bot.pri)
