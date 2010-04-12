include(../common_top.pri)
include(../common_msettings.pri)

TEST_SOURCES = \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagetextfactory.cpp \

# unit test and unit
SOURCES += \
    ut_msettingslanguagetextfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_msettingslanguagetextfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagetextfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagetextcontroller.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
