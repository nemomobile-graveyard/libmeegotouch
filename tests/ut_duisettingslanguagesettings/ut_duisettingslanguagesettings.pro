include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagesettings.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagesettings.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagesettings.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagesettings.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
