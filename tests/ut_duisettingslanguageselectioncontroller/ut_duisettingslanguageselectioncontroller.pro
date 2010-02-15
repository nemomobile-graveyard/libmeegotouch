include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/settingslanguage

TARGET = ut_duisettingslanguageselectioncontroller

# unit test and unit
SOURCES += \
    ut_duisettingslanguageselectioncontroller.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguageselectioncontroller.cpp

# unit test and unit
HEADERS += \
    ut_duisettingslanguageselectioncontroller.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageselectioncontroller.h

include(../common_bot.pri)

