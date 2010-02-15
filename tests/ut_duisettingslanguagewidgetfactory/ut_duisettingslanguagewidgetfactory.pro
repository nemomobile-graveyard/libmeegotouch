include(../common_top.pri)
TARGET = ut_duisettingslanguagewidgetfactory
INCLUDEPATH += $$DUISRCDIR/settingslanguage

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagewidgetfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagewidgetfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagewidgetfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagewidgetfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
