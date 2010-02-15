include(../common_top.pri)
TARGET = ut_duisettingslanguageinteger
INCLUDEPATH += $$DUISRCDIR/settingslanguage

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguageinteger.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguageinteger.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguageinteger.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageinteger.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
