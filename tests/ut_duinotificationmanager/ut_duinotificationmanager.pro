include(../common_top.pri)
TARGET = ut_duinotificationmanager
INCLUDEPATH += $$DUISRCDIR/corelib/notification

# unit test and unit
SOURCES += \
    ut_duinotificationmanager.cpp \
    $$DUISRCDIR/corelib/notification/duinotificationmanager.cpp \
    $$DUISRCDIR/corelib/notification/duinotification.cpp \
    $$DUISRCDIR/corelib/notification/duinotificationgroup.cpp

# unit test and unit
HEADERS += \
    ut_duinotificationmanager.h \
    $$DUISRCDIR/corelib/notification/duinotificationmanager.h \
    $$DUISRCDIR/corelib/notification/duinotification.h \
    $$DUISRCDIR/corelib/notification/duinotificationgroup.h

# service classes
HEADERS += \
    $$DUISRCDIR/corelib/notification/duinotificationmanagerproxy.h

include(../common_bot.pri)
