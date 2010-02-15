include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/settingslanguage

TARGET = ut_duisettingslanguageboolean

# unit test and unit
SOURCES += \
    ut_duisettingslanguageboolean.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguageboolean.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguageboolean.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageboolean.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
