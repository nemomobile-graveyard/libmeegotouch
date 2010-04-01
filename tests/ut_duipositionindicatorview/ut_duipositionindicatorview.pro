include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/corelib/widgets $$DUISRCDIR/corelib/style

TARGET = ut_duipositionindicatorview
CONFIG += DEBUG

# unit test and unit classes
SOURCES += \
    ut_duipositionindicatorview.cpp \

# unit test and unit classes
HEADERS += \
    ut_duipositionindicatorview.h \

include(../common_bot.pri)
