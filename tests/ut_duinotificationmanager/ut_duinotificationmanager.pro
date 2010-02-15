include(../common_top.pri)
TARGET = ut_duinotificationmanager
INCLUDEPATH += $$DUISRCDIR/notification \
    $$DUISRCDIR/mashup/mashup

# unit test and unit
SOURCES += \
    ut_duinotificationmanager.cpp \
    $$DUISRCDIR/notification/duinotificationmanager.cpp \
    $$DUISRCDIR/notification/duinotification.cpp \
    $$DUISRCDIR/notification/duinotificationgroup.cpp

# unit test and unit
HEADERS += \
    ut_duinotificationmanager.h \
    $$DUISRCDIR/notification/duinotificationmanager.h \
    $$DUISRCDIR/notification/duinotification.h \
    $$DUISRCDIR/notification/duinotificationgroup.h

# service classes
HEADERS += \
    $$DUISRCDIR/notification/duinotificationmanagerproxy.h

include(../common_bot.pri)
