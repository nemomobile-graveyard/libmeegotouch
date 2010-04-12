include(../common_top.pri)
include(../common_msettings.pri)

QT += xml

# unit test and unit
SOURCES += \
    ut_msettingslanguageparser.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageparser.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebinary.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_msettingslanguageparser.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageparser.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebinary.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)
