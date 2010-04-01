include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagebooleanfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebooleanfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagebooleanfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebooleanfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebooleancontroller.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
