include(../common_top.pri)
TARGET = ut_mremoteaction
INCLUDEPATH += $$MSRCDIR/corelib/core

TEST_SOURCES = \
    $$MSRCDIR/corelib/core/mremoteaction.cpp \

# unit test and unit
SOURCES += \
    ut_mremoteaction.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mremoteaction.h \
    $$MSRCDIR/corelib/core/maction.h \
    $$MSRCDIR/corelib/core/mremoteaction.h

include(../common_bot.pri)
