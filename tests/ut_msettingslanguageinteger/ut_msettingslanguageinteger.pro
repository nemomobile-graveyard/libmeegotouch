include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit classes
SOURCES += \
    ut_msettingslanguageinteger.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageinteger.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_msettingslanguageinteger.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageinteger.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
