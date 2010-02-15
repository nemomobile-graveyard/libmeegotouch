include(../common_top.pri)
QT += xml

INCLUDEPATH += $$DUISRCDIR/settingslanguage

TARGET = ut_duisettingslanguageparser

# unit test and unit
SOURCES += \
    ut_duisettingslanguageparser.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguageparser.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebinary.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguageparser.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageparser.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebinary.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
