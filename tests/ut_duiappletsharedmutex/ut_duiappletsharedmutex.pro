include(../common_top.pri)
TARGET = ut_duiappletsharedmutex
INCLUDEPATH += $$DUISRCDIR/mashup/mashup

TEST_SOURCES = \
    $$DUISRCDIR/mashup/mashup/duiappletsharedmutex.cpp \

# unit test and unit
SOURCES += \
    ut_duiappletsharedmutex.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duiappletsharedmutex.h \
    $$DUISRCDIR/mashup/mashup/duiappletsharedmutex.h \

include(../common_bot.pri)
