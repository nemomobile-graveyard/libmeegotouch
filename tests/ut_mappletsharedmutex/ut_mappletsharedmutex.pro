include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/extensions/mashup/mashup

TEST_SOURCES = \
    $$MSRCDIR/extensions/mashup/mashup/mappletsharedmutex.cpp \

# unit test and unit
SOURCES += \
    ut_mappletsharedmutex.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mappletsharedmutex.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletsharedmutex.h \

include(../common_bot.pri)
