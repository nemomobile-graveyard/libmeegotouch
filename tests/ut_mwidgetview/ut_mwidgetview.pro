include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

TARGET = ut_mwidgetview

# unit test and unit
SOURCES += \
    ut_mwidgetview.cpp \

# unit test and unit
HEADERS += \
    ut_mwidgetview.h \

include(../common_bot.pri)
