include(../common_top.pri)
include(../common_duisettings.pri)

TEST_SOURCES = \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagetextfactory.cpp \

# unit test and unit
SOURCES += \
    ut_duisettingslanguagetextfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguagetextfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagetextfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagetextcontroller.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
