include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/settingslanguage

TARGET = ut_duisettingslanguageoption

# unit test and unit
SOURCES += \
    ut_duisettingslanguageoption.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguageoption.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguageoption.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageoption.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
