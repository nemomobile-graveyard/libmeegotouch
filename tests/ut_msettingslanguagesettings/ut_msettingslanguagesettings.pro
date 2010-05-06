include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit classes
SOURCES += \
    ut_msettingslanguagesettings.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagesettings.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_msettingslanguagesettings.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagesettings.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
