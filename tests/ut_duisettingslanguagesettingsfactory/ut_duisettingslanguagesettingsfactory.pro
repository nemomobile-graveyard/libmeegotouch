include(../common_top.pri)
include(../common_duisettings.pri)

INCLUDEPATH += $$DUISRCDIR/corelib/style

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagesettingsfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagesettingsfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagesettingsfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagesettingsfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
