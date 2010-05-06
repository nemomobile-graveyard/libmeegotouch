include(../common_top.pri)
include(../common_msettings.pri)

# unit test and unit
SOURCES += \
    ut_msettingslanguagebooleancontroller.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageboolean.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebooleancontroller.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebooleanfactory.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.cpp

# unit test and unit
HEADERS += \
    ut_msettingslanguagebooleancontroller.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguageboolean.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebooleancontroller.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagebooleanfactory.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagenode.h

include(../common_bot.pri)

