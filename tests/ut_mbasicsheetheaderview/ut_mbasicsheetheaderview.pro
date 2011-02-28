include(../common_top.pri)
TARGET = ut_mbasicsheetheaderview

INCLUDEPATH += $$MSRCDIR/views $$STUBSDIR $$MSRCDIR/corelib/widgets

TEST_SOURCES = \
    $$MSRCDIR/views/mbasicsheetheaderview.cpp \

# unit test and unit
SOURCES += \
    ut_mbasicsheetheaderview.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mbasicsheetheaderview.h \
    $$MSRCDIR/views/mbasicsheetheaderview.h \
    $$MSRCDIR/views/mbasicsheetheaderview_p.h \

include(../common_bot.pri)
