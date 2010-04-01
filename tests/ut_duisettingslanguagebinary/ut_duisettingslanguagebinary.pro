include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/settings/settingslanguage

# unit test and unit
SOURCES += \
    ut_duisettingslanguagebinary.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebinary.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagesettings.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguagebinary.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebinary.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagesettings.h

include(../common_bot.pri)
