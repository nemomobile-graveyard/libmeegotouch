include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style $$MSRCDIR/views

TARGET = ut_mdialogview

# unit test and unit
SOURCES += \
    ut_mdialogview.cpp \

# unit test and unit
HEADERS += \
    ut_mdialogview.h \

include(../common_bot.pri)
