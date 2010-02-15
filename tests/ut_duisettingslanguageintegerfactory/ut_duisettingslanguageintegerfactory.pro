include(../common_top.pri)
TARGET = ut_duisettingslanguageintegerfactory
INCLUDEPATH += $$DUISRCDIR/settingslanguage

TEST_SOURCES = \
    $$DUISRCDIR/settingslanguage/duisettingslanguageintegerfactory.cpp \

# unit test and unit
SOURCES += \
    ut_duisettingslanguageintegerfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duisettingslanguageintegerfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageintegerfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageintegercontroller.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h \

include(../common_bot.pri)
