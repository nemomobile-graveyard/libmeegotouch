###############################################################################
# events and gestures module. Separated to events folder, used for multitouch
# prototype.
############################################################################### 
EVENTS_SRC_DIR= ./events
INCLUDEPATH+=./events
PRIVATE_HEADERS += \
	$$EVENTS_SRC_DIR/duiorientationtracker.h \
        $$EVENTS_SRC_DIR/duiorientationtracker_p.h \
        $$EVENTS_SRC_DIR/duiscenewindowevent_p.h \

HEADERS += \
        $$EVENTS_SRC_DIR/duicancelevent.h \
        $$EVENTS_SRC_DIR/duidismissevent.h \
        $$EVENTS_SRC_DIR/duipreeditinjectionevent.h \
        $$EVENTS_SRC_DIR/duiondisplaychangeevent.h \
        $$EVENTS_SRC_DIR/duiorientationchangeevent.h \

SOURCES += \
	$$EVENTS_SRC_DIR/duiorientationtracker.cpp \
	$$EVENTS_SRC_DIR/duicancelevent.cpp \
        $$EVENTS_SRC_DIR/duidismissevent.cpp \
    	$$EVENTS_SRC_DIR/duipreeditinjectionevent.cpp \
        $$EVENTS_SRC_DIR/duiondisplaychangeevent.cpp \
    	$$EVENTS_SRC_DIR/duiorientationchangeevent.cpp \
        $$EVENTS_SRC_DIR/duiscenewindowevent_p.cpp \
