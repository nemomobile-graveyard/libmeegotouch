include(../common_top.pri)

INCLUDEPATH += $$DUISRCDIR/settingslanguage

TARGET = ut_duisettingslanguagebooleancontroller

# unit test and unit
SOURCES += \
    ut_duisettingslanguagebooleancontroller.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguageboolean.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebooleancontroller.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebooleanfactory.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.cpp

# unit test and unit
HEADERS += \
    ut_duisettingslanguagebooleancontroller.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguageboolean.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebooleancontroller.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagebooleanfactory.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)

