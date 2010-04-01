include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguageinteger.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageinteger.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguageinteger.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageinteger.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
