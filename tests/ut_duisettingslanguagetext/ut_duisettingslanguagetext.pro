include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagetext.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagetext.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagetext.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagetext.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
