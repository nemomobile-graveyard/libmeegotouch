include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style \
    $$MSRCDIR/corelib/core \
    $$MSRCDIR/extensions

# unit test and unit classes
SOURCES += \
    ut_mapplicationextensionarea.cpp \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionarea.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionarea.cpp \
    $$MSRCDIR/extensions/mextensionwatcher.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_mapplicationextensionarea.h \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionarea.h \
    $$MSRCDIR/extensions/applicationextension/mextensionarea.h \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionmanager.h \
    $$MSRCDIR/extensions/mextensionwatcher.h

include(../common_bot.pri)
