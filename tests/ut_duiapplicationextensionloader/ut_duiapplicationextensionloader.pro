include(../common_top.pri)
include(../common_duiextensions.pri)

TEST_SOURCES = \
    $$DUISRCDIR/extensions/applicationextension/duiapplicationextensionloader.cpp \

# unit test and unit
SOURCES += \
    ut_duiapplicationextensionloader.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duiapplicationextensionloader.h \
    $$DUISRCDIR/extensions/applicationextension/duiapplicationextensionloader.h \

include(../common_bot.pri)
