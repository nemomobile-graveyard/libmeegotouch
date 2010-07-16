include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/events
TARGET = ut_mtapandholdrecognizer

# unit test and unit classes
SOURCES += \
    ut_mtapandholdrecognizer.cpp \
    ../../src/corelib/events/mtapandholdrecognizer.cpp \
    ../../src/corelib/events/mtapandholdgesture.cpp \
    
# unit test and unit classes
HEADERS += \
    ut_mtapandholdrecognizer.h \
    ../../src/corelib/events/mtapandholdgesture_p.h \

include(../common_bot.pri)
