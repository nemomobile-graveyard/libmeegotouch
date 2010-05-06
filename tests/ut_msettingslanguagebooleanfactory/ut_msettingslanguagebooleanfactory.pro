include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit classes
SOURCES += \
    ut_msettingslanguagebooleanfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebooleanfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_msettingslanguagebooleanfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebooleanfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebooleancontroller.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
