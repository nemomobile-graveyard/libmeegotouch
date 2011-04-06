###############################################################################
# Core module
# This module contains all classes that represent the most basic foundation
# of an application and have little to no relationship to the view classes.
###############################################################################
CORE_SRC_DIR=./core
INCLUDEPATH+=./core

PUBLIC_HEADERS += \
    $$CORE_SRC_DIR/mapplication.h \
    $$CORE_SRC_DIR/maction.h \
    $$CORE_SRC_DIR/massembly.h \
    $$CORE_SRC_DIR/mcomponentdata.h \
    $$CORE_SRC_DIR/mlibrary.h \
    $$CORE_SRC_DIR/mwidgetaction.h \
    $$CORE_SRC_DIR/mdesktopentry.h \
    $$CORE_SRC_DIR/mnamespace.h \
    $$CORE_SRC_DIR/mexport.h \
    $$CORE_SRC_DIR/mshareddata.h \
    $$CORE_SRC_DIR/mgconfitem.h \
    $$CORE_SRC_DIR/mtimestamp.h \
    $$CORE_SRC_DIR/minputmethodstate.h \
    $$CORE_SRC_DIR/mabstractscroller.h \
    $$CORE_SRC_DIR/mscenewindowscroller.h \
    $$CORE_SRC_DIR/mpannableviewportscroller.h \
    $$CORE_SRC_DIR/mdebug.h \
    $$CORE_SRC_DIR/mdataaccess.h \
    $$CORE_SRC_DIR/mdatastore.h \
    $$CORE_SRC_DIR/mfiledatastore.h \

!win32:PUBLIC_HEADERS += \
    $$CORE_SRC_DIR/mcomponentcache.h

PRIVATE_HEADERS += \
    $$CORE_SRC_DIR/mclassfactory.h \
    $$CORE_SRC_DIR/massembly_p.h \
    $$CORE_SRC_DIR/mdesktopentry_p.h \
    $$CORE_SRC_DIR/minputmethodstate_p.h \
    $$CORE_SRC_DIR/minputwidgetrelocator.h \
    $$CORE_SRC_DIR/mscrollchain.h \
    $$CORE_SRC_DIR/mcpumonitor.h \
    $$CORE_SRC_DIR/mfiledatastore_p.h \
    $$CORE_SRC_DIR/msyslogclient.h \
    $$CORE_SRC_DIR/mgraphicssystemhelper.h \
    $$CORE_SRC_DIR/mdynamicpropertywatcher.h \
    $$CORE_SRC_DIR/mmetatypes.h \
    $$CORE_SRC_DIR/msystemdirectories.h \

contains(DEFINES, HAVE_DBUS) {
    PUBLIC_HEADERS += \
        $$CORE_SRC_DIR/mapplicationservice.h \
        $$CORE_SRC_DIR/mapplicationifadaptor.h \
        $$CORE_SRC_DIR/mapplicationifproxy.h \
        $$CORE_SRC_DIR/mremoteaction.h \

    PRIVATE_HEADERS += \
        $$CORE_SRC_DIR/mapplicationservice_p.h \
        $$CORE_SRC_DIR/mdbusservicewaiter_p.h \
        $$CORE_SRC_DIR/mdbusinterface.h \

}

SOURCES += \
    $$CORE_SRC_DIR/mapplication.cpp \
    $$CORE_SRC_DIR/maction.cpp \
    $$CORE_SRC_DIR/massembly.cpp \
    $$CORE_SRC_DIR/mcomponentdata.cpp \
    $$CORE_SRC_DIR/mlibrary.cpp \
    $$CORE_SRC_DIR/mwidgetaction.cpp \
    $$CORE_SRC_DIR/mdesktopentry.cpp \
    $$CORE_SRC_DIR/mclassfactory.cpp \
    $$CORE_SRC_DIR/mcpumonitor.cpp \
    $$CORE_SRC_DIR/mshareddata.cpp \
    $$CORE_SRC_DIR/mtimestamp.cpp \
    $$CORE_SRC_DIR/minputmethodstate.cpp \
    $$CORE_SRC_DIR/minputwidgetrelocator.cpp \
    $$CORE_SRC_DIR/mscrollchain.cpp \
    $$CORE_SRC_DIR/mabstractscroller.cpp \
    $$CORE_SRC_DIR/mscenewindowscroller.cpp \
    $$CORE_SRC_DIR/mpannableviewportscroller.cpp \
    $$CORE_SRC_DIR/mfiledatastore.cpp \
    $$CORE_SRC_DIR/mnamespace.cpp \
    $$CORE_SRC_DIR/msyslogclient.cpp \
    $$CORE_SRC_DIR/mgraphicssystemhelper.cpp \
    $$CORE_SRC_DIR/mdynamicpropertywatcher.cpp \
    $$CORE_SRC_DIR/mdebug.cpp \
    $$CORE_SRC_DIR/msystemdirectories.cpp \

!win32:SOURCES += \
    $$CORE_SRC_DIR/mcomponentcache.cpp

contains(DEFINES, HAVE_DBUS) {
  SOURCES += \
    $$CORE_SRC_DIR/mapplicationservice.cpp \
    $$CORE_SRC_DIR/mapplicationservice_p.cpp \
    $$CORE_SRC_DIR/mapplicationifadaptor.cpp \
    $$CORE_SRC_DIR/mapplicationifproxy.cpp \
    $$CORE_SRC_DIR/mremoteaction.cpp \
    $$CORE_SRC_DIR/mdbusservicewaiter.cpp \
    $$CORE_SRC_DIR/mdbusinterface.cpp \
}

contains(DEFINES, HAVE_GCONF) {
    SOURCES += $$CORE_SRC_DIR/mgconfitem.cpp
} else {
    SOURCES += $$CORE_SRC_DIR/mgconfitem_stub.cpp
}

