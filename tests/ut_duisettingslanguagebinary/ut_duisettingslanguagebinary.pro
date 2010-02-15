include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/settingslanguage

TARGET = ut_duisettingslanguagebinary

# unit test and unit
SOURCES += \
    ut_duisettingslanguagebinary.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebinary.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagesettings.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguagebinary.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebinary.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagesettings.h

include(../common_bot.pri)
