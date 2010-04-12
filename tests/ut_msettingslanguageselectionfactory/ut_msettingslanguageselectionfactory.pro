include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit classes
SOURCES += \
    ut_msettingslanguageselectionfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageselectionfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_msettingslanguageselectionfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageselectionfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageselectioncontroller.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
