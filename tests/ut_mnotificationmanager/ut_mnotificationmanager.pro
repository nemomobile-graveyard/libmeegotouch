include(../common_top.pri)
TARGET = ut_mnotificationmanager
INCLUDEPATH += $$MSRCDIR/corelib/notification

# unit test and unit
SOURCES += \
    ut_mnotificationmanager.cpp \
    $$MSRCDIR/corelib/notification/mnotificationmanager.cpp \
    $$MSRCDIR/corelib/notification/mnotification.cpp \
    $$MSRCDIR/corelib/notification/mnotificationgroup.cpp

# unit test and unit
HEADERS += \
    ut_mnotificationmanager.h \
    $$MSRCDIR/corelib/notification/mnotificationmanager.h \
    $$MSRCDIR/corelib/notification/mnotification.h \
    $$MSRCDIR/corelib/notification/mnotificationgroup.h

# service classes
HEADERS += \
    $$MSRCDIR/corelib/notification/mnotificationmanagerproxy.h

include(../common_bot.pri)
