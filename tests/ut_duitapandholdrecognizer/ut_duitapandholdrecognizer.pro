include(../common_top.pri)
INCLUDEPATH += $$DUISRCDIR/events
TARGET = ut_duitapandholdrecognizer

# unit test and unit classes
SOURCES += \
    ut_duitapandholdrecognizer.cpp \
    ../../src/events/duitapandholdrecognizer.cpp \

# unit test and unit classes
HEADERS += \
    ut_duitapandholdrecognizer.h \

include(../common_bot.pri)
