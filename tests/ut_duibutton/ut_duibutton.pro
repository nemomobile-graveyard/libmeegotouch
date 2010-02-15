include(../common_top.pri)
INCLUDEPATH += $$DUISRCDIR/widgets $$DUISRCDIR/style

TARGET = ut_duibutton

# unit
TEST_SOURCES =  \
#    $$DUISRCDIR/duibutton.cpp \

# unit test and unit
SOURCES += \
    ut_duibutton.cpp \

# unit test and unit
HEADERS += \
    ut_duibutton.h \

include(../common_bot.pri)
