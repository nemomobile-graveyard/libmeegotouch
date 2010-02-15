include(../common_top.pri)
TARGET = ut_duiremoteaction
INCLUDEPATH += $$DUISRCDIR/core

TEST_SOURCES = \
    $$DUISRCDIR/core/duiremoteaction.cpp \

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
    $$DUISRCDIR/core/duiaction.h \
    $$DUISRCDIR/core/duiremoteaction.h

include(../common_bot.pri)
