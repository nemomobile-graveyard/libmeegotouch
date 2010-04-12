include(../common_top.pri)
TARGET = ut_mapplicationmenuview

INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

# unit test and unit
SOURCES += \
    ut_mapplicationmenuview.cpp \

# unit test and unit
HEADERS += \
    ut_mapplicationmenuview.h \

include(../common_bot.pri)
