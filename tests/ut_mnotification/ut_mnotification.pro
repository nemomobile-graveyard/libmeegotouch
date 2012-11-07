include(../common_top.pri)
TARGET = ut_mnotification
INCLUDEPATH += $$MSRCDIR/corelib/notification

# unit test and unit
SOURCES += \
    ut_mnotification.cpp \
    $$MSRCDIR/corelib/notification/mnotification.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mnotification.h \
    $$MSRCDIR/corelib/notification/mnotification.h \
    $$MSRCDIR/corelib/notification/mnotification_p.h \
    $$MSRCDIR/corelib/notification/mnotificationmanagerproxy.h \
    $$MSRCDIR/corelib/core/mremoteaction.h \
    $$MSRCDIR/corelib/core/maction.h

include(../common_bot.pri)
