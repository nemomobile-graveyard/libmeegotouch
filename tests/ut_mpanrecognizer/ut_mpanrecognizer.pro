include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/events
TARGET = ut_mpanrecognizer

# unit test and unit classes
SOURCES += \
    ut_mpanrecognizer.cpp \
    ../../src/corelib/events/mpanrecognizer.cpp \
    ../../src/corelib/events/mpangesture.cpp \
    
# unit test and unit classes
HEADERS += \
    ut_mpanrecognizer.h \
    ../../src/corelib/events/mpangesture_p.h \

include(../common_bot.pri)
