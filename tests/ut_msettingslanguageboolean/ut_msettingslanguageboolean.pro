include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit
SOURCES += \
    ut_msettingslanguageboolean.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageboolean.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_msettingslanguageboolean.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageboolean.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
