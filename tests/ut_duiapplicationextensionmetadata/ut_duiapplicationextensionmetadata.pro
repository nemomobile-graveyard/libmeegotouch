include(../common_top.pri)
INCLUDEPATH += $$DUISRCDIR/applicationextension

TARGET = ut_duiapplicationextensionmetadata

TEST_SOURCES = \
    $$DUISRCDIR/applicationextension/duiapplicationextensionmetadata.cpp \

# unit test and unit
SOURCES += \
    ut_duiapplicationextensionmetadata.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duiapplicationextensionmetadata.h \
    $$DUISRCDIR/applicationextension/duiapplicationextensionmetadata.h \

DEFINES += DUI_INSTALL_LIBS=\\\"\"$$DUI_INSTALL_LIBS\"\\\"

include(../common_bot.pri)
