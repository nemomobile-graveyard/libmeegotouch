include(../common_top.pri)
TARGET = ut_duiapplicationmenuview

INCLUDEPATH += $$DUISRCDIR/widgets $$DUISRCDIR/style

# unit test and unit
SOURCES += \
    ut_duiapplicationmenuview.cpp \

# unit test and unit
HEADERS += \
    ut_duiapplicationmenuview.h \

include(../common_bot.pri)
