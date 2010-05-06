include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit
SOURCES += \
    ut_msettingslanguageoption.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageoption.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_msettingslanguageoption.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageoption.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
