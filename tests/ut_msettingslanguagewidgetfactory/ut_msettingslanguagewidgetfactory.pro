include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit classes
SOURCES += \
    ut_msettingslanguagewidgetfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagewidgetfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_msettingslanguagewidgetfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagewidgetfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
