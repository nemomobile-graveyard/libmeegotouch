include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit
SOURCES += \
    ut_duisettingslanguageoption.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageoption.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguageoption.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageoption.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
