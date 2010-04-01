include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit
SOURCES += \
    ut_duisettingslanguagetextcontroller.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagetextcontroller.cpp

# unit test and unit
HEADERS += \
    ut_duisettingslanguagetextcontroller.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagetextcontroller.h

include(../common_bot.pri)

