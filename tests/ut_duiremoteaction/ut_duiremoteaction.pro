include(../common_top.pri)
TARGET = ut_duiremoteaction
INCLUDEPATH += $$DUISRCDIR/corelib/core

TEST_SOURCES = \
    $$DUISRCDIR/corelib/core/duiremoteaction.cpp \

# unit test and unit
SOURCES += \
    ut_duiremoteaction.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duiremoteaction.h \
    $$DUISRCDIR/corelib/core/duiaction.h \
    $$DUISRCDIR/corelib/core/duiremoteaction.h

include(../common_bot.pri)
