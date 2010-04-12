include(../common_top.pri)
include(../common_msettings.pri)

INCLUDEPATH += $$MSRCDIR/corelib/style

# unit test and unit classes
SOURCES += \
    ut_msettingslanguagesettingsfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagesettingsfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_msettingslanguagesettingsfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagesettingsfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
