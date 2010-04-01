include(../common_top.pri)
include(../common_duisettings.pri)

QT += xml

# unit test and unit
SOURCES += \
    ut_duisettingslanguageparser.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageparser.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebinary.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguageparser.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageparser.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebinary.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
