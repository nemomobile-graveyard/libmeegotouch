include(../common_top.pri)
TARGET = ut_duisettingslanguagesettings
INCLUDEPATH += $$DUISRCDIR/settingslanguage

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagesettings.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagesettings.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagesettings.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagesettings.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
