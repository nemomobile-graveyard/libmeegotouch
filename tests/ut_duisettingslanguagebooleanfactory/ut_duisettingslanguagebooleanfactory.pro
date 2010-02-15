include(../common_top.pri)
TARGET = ut_duisettingslanguagebooleanfactory
INCLUDEPATH += $$DUISRCDIR/settingslanguage

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagebooleanfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebooleanfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagebooleanfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebooleanfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebooleancontroller.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
