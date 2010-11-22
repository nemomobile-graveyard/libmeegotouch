include(../common_top.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/core \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style \
    $$MSRCDIR/views

TARGET = ut_mnavigationbarview

# unit test and unit
SOURCES += \
    ut_mnavigationbarview.cpp \

# unit test and unit
HEADERS += \
    ut_mnavigationbarview.h \

include(../common_bot.pri)