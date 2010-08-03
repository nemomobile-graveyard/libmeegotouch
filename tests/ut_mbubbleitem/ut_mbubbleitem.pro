include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

TARGET = ut_mbubbleitem

# unit
TEST_SOURCES =  \
#    $$MSRCDIR/mbubbleitem.cpp \

# unit test and unit
SOURCES += \
    ut_mbubbleitem.cpp \

# unit test and unit
HEADERS += \
    ut_mbubbleitem.h \

include(../common_bot.pri)
