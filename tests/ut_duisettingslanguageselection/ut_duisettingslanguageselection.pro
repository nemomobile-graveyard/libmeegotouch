include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/settingslanguage

TARGET = ut_duisettingslanguageselection

# unit test and unit
SOURCES += \
    ut_duisettingslanguageselection.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguageselection.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguageselection.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageselection.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageoption.h

include(../common_bot.pri)
