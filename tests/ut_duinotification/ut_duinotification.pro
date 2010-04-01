include(../common_top.pri)
TARGET = ut_duinotification
INCLUDEPATH += $$DUISRCDIR/corelib/notification

# unit test and unit
SOURCES += \
    ut_duinotification.cpp \
    $$DUISRCDIR/corelib/notification/duinotification.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duinotification.h \
    $$DUISRCDIR/corelib/notification/duinotification.h \
    $$DUISRCDIR/corelib/notification/duinotificationmanager.h \
    $$DUISRCDIR/corelib/notification/duinotificationmanagerproxy.h \
    $$DUISRCDIR/corelib/core/duiremoteaction.h \
    $$DUISRCDIR/corelib/core/duiaction.h

include(../common_bot.pri)
