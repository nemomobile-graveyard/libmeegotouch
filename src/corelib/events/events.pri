###############################################################################
# events and gestures module. Separated to events folder, used for multitouch
# prototype.
###############################################################################
EVENTS_SRC_DIR= ./events
INCLUDEPATH+=./events
PUBLIC_HEADERS += \
        $$EVENTS_SRC_DIR/mcancelevent.h \
        $$EVENTS_SRC_DIR/mdismissevent.h \
        $$EVENTS_SRC_DIR/mpreeditinjectionevent.h \
        $$EVENTS_SRC_DIR/mondisplaychangeevent.h \
        $$EVENTS_SRC_DIR/morientationchangeevent.h \
        $$EVENTS_SRC_DIR/morientationtracker.h \
        $$EVENTS_SRC_DIR/mtapandholdrecognizer.h \
        $$EVENTS_SRC_DIR/mswiperecognizer.h \
        $$EVENTS_SRC_DIR/mkeyboardstatetracker.h \

PRIVATE_HEADERS += \
        $$EVENTS_SRC_DIR/morientationchangeevent_p.h \
        $$EVENTS_SRC_DIR/morientationtracker_p.h \
        $$EVENTS_SRC_DIR/mscenewindowevent_p.h \
        $$EVENTS_SRC_DIR/mtapandholdrecognizer_p.h \
        $$EVENTS_SRC_DIR/mtapandholdgesture_p.h \
        $$EVENTS_SRC_DIR/mkeyboardstatetracker_p.h \
        $$EVENTS_SRC_DIR/mpangesture_p.h \
        $$EVENTS_SRC_DIR/mpanrecognizer_p.h \
        $$EVENTS_SRC_DIR/mpanrecognizer.h \
        $$EVENTS_SRC_DIR/mswipegesture_p.h \
        $$EVENTS_SRC_DIR/mswiperecognizer_p.h \
        $$EVENTS_SRC_DIR/mswiperecognizer.h \

contains(DEFINES, HAVE_DBUS) {
        PRIVATE_HEADERS += \
                $$EVENTS_SRC_DIR/mservicelistener.h
}

SOURCES += \
        $$EVENTS_SRC_DIR/morientationtracker.cpp \
        $$EVENTS_SRC_DIR/mcancelevent.cpp \
        $$EVENTS_SRC_DIR/mdismissevent.cpp \
        $$EVENTS_SRC_DIR/mpreeditinjectionevent.cpp \
        $$EVENTS_SRC_DIR/mondisplaychangeevent.cpp \
        $$EVENTS_SRC_DIR/morientationchangeevent.cpp \
        $$EVENTS_SRC_DIR/mscenewindowevent_p.cpp \
        $$EVENTS_SRC_DIR/mtapandholdgesture.cpp \
        $$EVENTS_SRC_DIR/mtapandholdrecognizer.cpp \
        $$EVENTS_SRC_DIR/mkeyboardstatetracker.cpp \
        $$EVENTS_SRC_DIR/mpanrecognizer.cpp \
        $$EVENTS_SRC_DIR/mpangesture.cpp \
        $$EVENTS_SRC_DIR/mswiperecognizer.cpp \
        $$EVENTS_SRC_DIR/mswipegesture.cpp \

contains(DEFINES, HAVE_DBUS) {
        SOURCES += \
                $$EVENTS_SRC_DIR/mservicelistener.cpp
}
