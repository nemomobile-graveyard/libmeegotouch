include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit
SOURCES += \
    ut_duisettingslanguageselectioncontroller.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageselectioncontroller.cpp

# unit test and unit
HEADERS += \
    ut_duisettingslanguageselectioncontroller.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageselectioncontroller.h

include(../common_bot.pri)

