include(../common_top.pri)
include(../common_duisettings.pri)

TEST_SOURCES = \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageintegerfactory.cpp \

# unit test and unit
SOURCES += \
    ut_duisettingslanguageintegerfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguageintegerfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageintegerfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageintegercontroller.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h \

include(../common_bot.pri)
