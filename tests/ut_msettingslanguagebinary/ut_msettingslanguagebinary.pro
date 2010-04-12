include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/settings/settingslanguage

# unit test and unit
SOURCES += \
    ut_msettingslanguagebinary.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebinary.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagesettings.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_msettingslanguagebinary.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebinary.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagesettings.h

include(../common_bot.pri)
