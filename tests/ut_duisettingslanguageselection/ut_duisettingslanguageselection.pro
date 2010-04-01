include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit
SOURCES += \
    ut_duisettingslanguageselection.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageselection.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguageselection.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageselection.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageoption.h

include(../common_bot.pri)
