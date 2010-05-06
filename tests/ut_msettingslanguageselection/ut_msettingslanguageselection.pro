include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit
SOURCES += \
    ut_msettingslanguageselection.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageselection.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_msettingslanguageselection.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageselection.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageoption.h

include(../common_bot.pri)
