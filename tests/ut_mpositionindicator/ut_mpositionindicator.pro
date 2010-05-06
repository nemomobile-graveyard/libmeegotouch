include(../common_top.pri)

TARGET = ut_mpositionindicator


TEST_SOURCES += \ 
#    $$MSRCDIR/mpositionindicator.cpp \

# unit test and unit classes
SOURCES += \
    ut_mpositionindicator.cpp \
#    $$TEST_SOURCES \

# unit test and unit classes
HEADERS += \
    ut_mpositionindicator.h \

include(../common_bot.pri)
