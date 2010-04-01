include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguageselectionfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageselectionfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguageselectionfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageselectionfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageselectioncontroller.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
