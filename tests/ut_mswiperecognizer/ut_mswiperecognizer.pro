include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/events
TARGET = ut_mswiperecognizer

# unit test and unit classes
SOURCES += \
    ut_mswiperecognizer.cpp \
    $$MSRCDIR/corelib/events/mswiperecognizer.cpp \
    $$MSRCDIR/corelib/events/mswipegesture.cpp \
    
# unit test and unit classes
HEADERS += \
    ut_mswiperecognizer.h \
    $$MSRCDIR/corelib/events/mswipegesture_p.h \

include(../common_bot.pri)
