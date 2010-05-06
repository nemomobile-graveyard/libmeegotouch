include(../common_top.pri)
include(../common_msettings.pri)

TEST_SOURCES = \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageintegerfactory.cpp \

# unit test and unit
SOURCES += \
    ut_msettingslanguageintegerfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_msettingslanguageintegerfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageintegerfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageintegercontroller.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h \

include(../common_bot.pri)
