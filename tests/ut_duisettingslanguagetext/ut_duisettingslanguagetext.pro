include(../common_top.pri)
TARGET = ut_duisettingslanguagetext
INCLUDEPATH += $$DUISRCDIR/settingslanguage

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagetext.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagetext.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagetext.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagetext.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
