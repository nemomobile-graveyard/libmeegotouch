include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/applicationextension

TARGET = ut_mapplicationextensionmetadata

TEST_SOURCES = \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionmetadata.cpp \

# unit test and unit
SOURCES += \
    ut_mapplicationextensionmetadata.cpp \
    $$TEST_SOURCES \ 

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mapplicationextensionmetadata.h \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionmetadata.h \

DEFINES += M_INSTALL_LIBS=\\\"\"$$M_INSTALL_LIBS\"\\\"

include(../common_bot.pri)
