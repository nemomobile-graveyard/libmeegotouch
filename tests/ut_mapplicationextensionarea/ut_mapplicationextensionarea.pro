include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style \
    $$MSRCDIR/corelib/core \

# unit test and unit classes
SOURCES += \
    ut_mapplicationextensionarea.cpp \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionarea.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionarea.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_mapplicationextensionarea.h \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionarea.h \
    $$MSRCDIR/extensions/applicationextension/mextensionarea.h \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionmanager.h 

include(../common_bot.pri)
