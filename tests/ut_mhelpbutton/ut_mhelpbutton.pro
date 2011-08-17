include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \

TARGET = ut_mhelpbutton

# unit test and unit
SOURCES += \
    ut_mhelpbutton.cpp \

# unit test and unit
HEADERS += \
    ut_mhelpbutton.h \

include(../common_bot.pri)
