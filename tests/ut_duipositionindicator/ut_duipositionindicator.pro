include(../common_top.pri)

TARGET = ut_duipositionindicator


TEST_SOURCES += \ 
#    $$DUISRCDIR/duipositionindicator.cpp \

# unit test and unit classes
SOURCES += \
    ut_duipositionindicator.cpp \
#    $$TEST_SOURCES \

# unit test and unit classes
HEADERS += \
    ut_duipositionindicator.h \

include(../common_bot.pri)
