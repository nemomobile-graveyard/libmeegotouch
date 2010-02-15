include(../common_top.pri)
TARGET = ut_duisettingslanguageselectionfactory
INCLUDEPATH += $$DUISRCDIR/settingslanguage

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguageselectionfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguageselectionfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguageselectionfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageselectionfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageselectioncontroller.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
