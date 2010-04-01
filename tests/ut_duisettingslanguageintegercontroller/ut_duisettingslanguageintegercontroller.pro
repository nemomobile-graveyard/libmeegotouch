include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit
SOURCES += \
    ut_duisettingslanguageintegercontroller.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageintegercontroller.cpp

# unit test and unit
HEADERS += \
    ut_duisettingslanguageintegercontroller.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageintegercontroller.h

include(../common_bot.pri)

