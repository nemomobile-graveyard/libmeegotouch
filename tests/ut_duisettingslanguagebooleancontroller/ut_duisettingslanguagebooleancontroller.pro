include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit
SOURCES += \
    ut_duisettingslanguagebooleancontroller.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageboolean.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebooleancontroller.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebooleanfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# unit test and unit
HEADERS += \
    ut_duisettingslanguagebooleancontroller.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguageboolean.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebooleancontroller.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagebooleanfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)

