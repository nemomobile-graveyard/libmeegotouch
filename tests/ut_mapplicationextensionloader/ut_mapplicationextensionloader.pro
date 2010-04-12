include(../common_top.pri)
include(../common_mextensions.pri)

TEST_SOURCES = \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionloader.cpp \

# unit test and unit
SOURCES += \
    ut_mapplicationextensionloader.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mapplicationextensionloader.h \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionloader.h \

include(../common_bot.pri)
