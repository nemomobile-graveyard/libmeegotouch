include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit
SOURCES += \
    ut_msettingslanguageselectioncontroller.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageselectioncontroller.cpp

# unit test and unit
HEADERS += \
    ut_msettingslanguageselectioncontroller.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageselectioncontroller.h

include(../common_bot.pri)

