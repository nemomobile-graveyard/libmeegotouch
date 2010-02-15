include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/settingslanguage

TARGET = ut_duisettingslanguagetextcontroller

# unit test and unit
SOURCES += \
    ut_duisettingslanguagetextcontroller.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagetextcontroller.cpp

# unit test and unit
HEADERS += \
    ut_duisettingslanguagetextcontroller.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagetextcontroller.h

include(../common_bot.pri)

