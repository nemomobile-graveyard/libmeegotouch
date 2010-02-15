include(../common_top.pri)
TARGET = ut_duinotification
INCLUDEPATH += $$DUISRCDIR/notification

# unit test and unit
SOURCES += \
    ut_duinotification.cpp \
    $$DUISRCDIR/notification/duinotification.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_duinotification.h \
    $$DUISRCDIR/notification/duinotification.h \
    $$DUISRCDIR/notification/duinotificationmanager.h \
    $$DUISRCDIR/notification/duinotificationmanagerproxy.h \
    $$DUISRCDIR/core/duiremoteaction.h \
    $$DUISRCDIR/core/duiaction.h

include(../common_bot.pri)
