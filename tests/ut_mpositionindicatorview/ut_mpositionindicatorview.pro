include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

TARGET = ut_mpositionindicatorview
CONFIG += DEBUG

# unit test and unit classes
SOURCES += \
    ut_mpositionindicatorview.cpp \

# unit test and unit classes
HEADERS += \
    ut_mpositionindicatorview.h \

include(../common_bot.pri)
