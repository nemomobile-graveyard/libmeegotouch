include(../common_top.pri)
INCLUDEPATH += $$DUISRCDIR/corelib/events
TARGET = ut_duitapandholdrecognizer

# unit test and unit classes
SOURCES += \
    ut_duitapandholdrecognizer.cpp \
    ../../src/corelib/events/duitapandholdrecognizer.cpp \
    ../../src/corelib/events/duitapandholdgesture.cpp \
    
# unit test and unit classes
HEADERS += \
    ut_duitapandholdrecognizer.h \
    ../../src/corelib/events/duitapandholdgesture_p.h \

include(../common_bot.pri)
