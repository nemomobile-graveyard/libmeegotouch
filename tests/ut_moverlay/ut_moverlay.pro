include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets $$MSRCDIR/corelib/style

TARGET = ut_moverlay

# unit
TEST_SOURCES =  \
#    $$MSRCDIR/moverlay.cpp \

# unit test and unit
SOURCES += \
    ut_moverlay.cpp \

# unit test and unit
HEADERS += \
    ut_moverlay.h \

include(../common_bot.pri)
