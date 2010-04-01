include(../common_top.pri)
INCLUDEPATH += $$DUISRCDIR/corelib/widgets $$DUISRCDIR/corelib/style

TARGET = ut_duiobjectmenu

# unit
TEST_SOURCES =  \

# unit test and unit
SOURCES += \
    ut_duiobjectmenu.cpp \

# unit test and unit
HEADERS += \
    ut_duiobjectmenu.h \

include(../common_bot.pri)
