###############################################################################
# DuiCore module
# This module contains all classes that represent the most basic foundation
# of an application and have little to no relationship to the view classes.
###############################################################################
CORE_SRC_DIR=./core
INCLUDEPATH+=./core
HEADERS += \
    $$CORE_SRC_DIR/duiapplication.h \
    $$CORE_SRC_DIR/duiaction.h \
    $$CORE_SRC_DIR/duiassembly.h \
    $$CORE_SRC_DIR/duiassembly_p.h \
    $$CORE_SRC_DIR/duicomponentdata.h \
    $$CORE_SRC_DIR/duilibrary.h \
    $$CORE_SRC_DIR/duiwidgetaction.h \
    $$CORE_SRC_DIR/duidesktopentry.h \
    $$CORE_SRC_DIR/duidesktopentry_p.h \
    $$CORE_SRC_DIR/duinamespace.h \
    $$CORE_SRC_DIR/duiexport.h \
    $$CORE_SRC_DIR/duiclassfactory.h \
    $$CORE_SRC_DIR/duicpumonitor.h \
    $$CORE_SRC_DIR/duishareddata.h \
    $$CORE_SRC_DIR/duigconfitem.h \
    $$CORE_SRC_DIR/duitimestamp.h \
    $$CORE_SRC_DIR/duiinputmethodstate.h \
    $$CORE_SRC_DIR/duiinputmethodstate_p.h \
    $$CORE_SRC_DIR/duidebug.h \

contains(DEFINES, HAVE_DBUS) {
  HEADERS += \
    $$CORE_SRC_DIR/duiapplicationservice.h \
    $$CORE_SRC_DIR/duiapplicationservice_p.h \
    $$CORE_SRC_DIR/duiapplicationifadaptor.h \
    $$CORE_SRC_DIR/duiapplicationifproxy.h \
    $$CORE_SRC_DIR/duiremoteaction.h \
    $$CORE_SRC_DIR/duidbusservicewaiter_p.h \
}

SOURCES += \
    $$CORE_SRC_DIR/duiapplication.cpp \
    $$CORE_SRC_DIR/duiaction.cpp \
    $$CORE_SRC_DIR/duiassembly.cpp \
    $$CORE_SRC_DIR/duicomponentdata.cpp \
    $$CORE_SRC_DIR/duilibrary.cpp \
    $$CORE_SRC_DIR/duiwidgetaction.cpp \
    $$CORE_SRC_DIR/duidesktopentry.cpp \
    $$CORE_SRC_DIR/duiclassfactory.cpp \
    $$CORE_SRC_DIR/duicpumonitor.cpp \
        $$CORE_SRC_DIR/duishareddata.cpp \
    $$CORE_SRC_DIR/duitimestamp.cpp \
    $$CORE_SRC_DIR/duiinputmethodstate.cpp \

contains(DEFINES, HAVE_DBUS) {
  SOURCES += \
    $$CORE_SRC_DIR/duiapplicationservice.cpp \
    $$CORE_SRC_DIR/duiapplicationservice_p.cpp \
    $$CORE_SRC_DIR/duiapplicationifadaptor.cpp \
    $$CORE_SRC_DIR/duiapplicationifproxy.cpp \
    $$CORE_SRC_DIR/duiremoteaction.cpp \
    $$CORE_SRC_DIR/duidbusservicewaiter.cpp \
}

contains(DEFINES, HAVE_GCONF) {
    SOURCES += $$CORE_SRC_DIR/duigconfitem.cpp
} else {
    SOURCES += $$CORE_SRC_DIR/duigconfitem_stub.cpp
}
