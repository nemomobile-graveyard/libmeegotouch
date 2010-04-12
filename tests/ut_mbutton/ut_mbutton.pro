include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

TARGET = ut_mbutton

# unit
TEST_SOURCES =  \
#    $$MSRCDIR/mbutton.cpp \

# unit test and unit
SOURCES += \
    ut_mbutton.cpp \

# unit test and unit
HEADERS += \
    ut_mbutton.h \

include(../common_bot.pri)
