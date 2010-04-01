include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/corelib/widgets $$DUISRCDIR/corelib/style

TARGET = ut_duidialogview

# unit test and unit
SOURCES += \
    ut_duidialogview.cpp \

# unit test and unit
HEADERS += \
    ut_duidialogview.h \

include(../common_bot.pri)
