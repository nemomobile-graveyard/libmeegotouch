include(../common_top.pri)
INCLUDEPATH += $$DUISRCDIR/corelib/widgets $$DUISRCDIR/corelib/style

TARGET = ut_duioverlay

# unit
TEST_SOURCES =  \
#    $$DUISRCDIR/duioverlay.cpp \

# unit test and unit
SOURCES += \
    ut_duioverlay.cpp \

# unit test and unit
HEADERS += \
    ut_duioverlay.h \

include(../common_bot.pri)
