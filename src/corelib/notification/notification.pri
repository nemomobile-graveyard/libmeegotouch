# ##############################################################################
# DuiNotification module
# This module contains a convenience API for handling notifications in a system
# ##############################################################################
NOTIFICATION_SRC_DIR = ./notification
INCLUDEPATH += $$NOTIFICATION_SRC_DIR

# These headers should get installed
PUBLIC_HEADERS += \
    $$NOTIFICATION_SRC_DIR/duinotification.h \
    $$NOTIFICATION_SRC_DIR/duinotificationgroup.h


PRIVATE_HEADERS += \
    $$NOTIFICATION_SRC_DIR/duinotification_p.h \
    $$NOTIFICATION_SRC_DIR/duinotificationgroup_p.h

# These headers should not get installed
PRIVATE_HEADERS += $$NOTIFICATION_SRC_DIR/duinotificationmanager.h \
    $$NOTIFICATION_SRC_DIR/duinotificationmanagerproxy.h \

SOURCES += \
    $$NOTIFICATION_SRC_DIR/duinotification.cpp \
    $$NOTIFICATION_SRC_DIR/duinotificationgroup.cpp \
    $$NOTIFICATION_SRC_DIR/duinotificationmanager.cpp \
    $$NOTIFICATION_SRC_DIR/duinotificationmanagerproxy.cpp
