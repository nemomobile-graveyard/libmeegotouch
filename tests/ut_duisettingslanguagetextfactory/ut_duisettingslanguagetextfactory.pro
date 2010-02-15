include(../common_top.pri)
TARGET = ut_duisettingslanguagetextfactory
INCLUDEPATH += $$DUISRCDIR/settingslanguage

TEST_SOURCES = \
    $$DUISRCDIR/settingslanguage/duisettingslanguagetextfactory.cpp \

# unit test and unit
SOURCES += \
    ut_duisettingslanguagetextfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguagetextfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagetextfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagetextcontroller.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
