include(../common_top.pri)
include(../common_duiextensions.pri)

INCLUDEPATH += \
    $$DUISRCDIR/corelib/widgets \
    $$DUISRCDIR/corelib/style \
    $$DUISRCDIR/corelib/core \

# unit test and unit classes
SOURCES += \
    ut_duiapplicationextensionarea.cpp \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
   ut_duiapplicationextensionarea.h \

include(../common_bot.pri)
