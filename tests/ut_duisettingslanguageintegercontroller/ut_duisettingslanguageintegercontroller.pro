include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/settingslanguage

TARGET = ut_duisettingslanguageintegercontroller

# unit test and unit
SOURCES += \
    ut_duisettingslanguageintegercontroller.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguageintegercontroller.cpp

# unit test and unit
HEADERS += \
    ut_duisettingslanguageintegercontroller.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageintegercontroller.h

include(../common_bot.pri)

