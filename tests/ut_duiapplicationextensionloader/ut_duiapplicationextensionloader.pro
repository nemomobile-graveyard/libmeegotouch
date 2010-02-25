include(../common_top.pri)
TARGET = ut_duiapplicationextensionloader
INCLUDEPATH += $$DUISRCDIR/applicationextension

TEST_SOURCES = \
    $$DUISRCDIR/applicationextension/duiapplicationextensionloader.cpp \

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
    $$DUISRCDIR/applicationextension/duiapplicationextensionloader.h \

include(../common_bot.pri)
