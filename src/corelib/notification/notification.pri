# ##############################################################################
# Notification module
# This module contains a convenience API for handling notifications in a system
# ##############################################################################
NOTIFICATION_SRC_DIR = ./notification
INCLUDEPATH += $$NOTIFICATION_SRC_DIR

# These headers should get installed
PUBLIC_HEADERS += \
    $$NOTIFICATION_SRC_DIR/mnotification.h \
    $$NOTIFICATION_SRC_DIR/mnotificationgroup.h


PRIVATE_HEADERS += \
    $$NOTIFICATION_SRC_DIR/mnotification_p.h \
    $$NOTIFICATION_SRC_DIR/mnotificationgroup_p.h

# These headers should not get installed
PRIVATE_HEADERS += \
    $$NOTIFICATION_SRC_DIR/mnotificationmanagerproxy.h

SOURCES += \
    $$NOTIFICATION_SRC_DIR/mnotification.cpp \
    $$NOTIFICATION_SRC_DIR/mnotificationgroup.cpp \
    $$NOTIFICATION_SRC_DIR/mnotificationmanagerproxy.cpp
