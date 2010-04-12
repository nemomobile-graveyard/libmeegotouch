include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style \
    $$MSRCDIR/corelib/core \

# unit test and unit classes
SOURCES += \
    ut_mapplicationextensionarea.cpp \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
   ut_mapplicationextensionarea.h \

include(../common_bot.pri)
