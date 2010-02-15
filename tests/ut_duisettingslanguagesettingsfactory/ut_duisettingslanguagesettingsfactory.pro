include(../common_top.pri)
TARGET = ut_duisettingslanguagesettingsfactory
INCLUDEPATH += $$DUISRCDIR/settingslanguage $$DUISRCDIR/style

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagesettingsfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagesettingsfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagesettingsfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagesettingsfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
