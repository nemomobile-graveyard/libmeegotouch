include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/widgets $$DUISRCDIR/style

TARGET = ut_duiwidgetview

# unit test and unit
SOURCES += \
    ut_duiwidgetview.cpp \

# unit test and unit
HEADERS += \
    ut_duiwidgetview.h \

include(../common_bot.pri)
