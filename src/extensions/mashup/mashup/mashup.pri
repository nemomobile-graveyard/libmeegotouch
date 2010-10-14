include(../mashup.pri)
MASHUP_MASHUP_SRC_DIR = $$MASHUP_SRC_DIR/mashup

PUBLIC_HEADERS += \
    $$MASHUP_MASHUP_SRC_DIR/mappletsharedmutex.h \
    $$MASHUP_MASHUP_SRC_DIR/maggregatedataaccess.h \
    $$MASHUP_MASHUP_SRC_DIR/mmashupcanvas.h \
    $$MASHUP_MASHUP_SRC_DIR/mmashupcanvasview.h \
    $$MASHUP_MASHUP_SRC_DIR/msubdatastore.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletloader.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletsettings.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletsettingsdialog.h \
    $$MASHUP_MASHUP_SRC_DIR/mgconfdatastore.h \

PRIVATE_HEADERS += $$MASHUP_MASHUP_SRC_DIR/mappletsharedmutex_p.h \
    $$MASHUP_MASHUP_SRC_DIR/mapplethandle_p.h \
    $$MASHUP_MASHUP_SRC_DIR/mapplethandleview_p.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletinventoryview_p.h \
    $$MASHUP_MASHUP_SRC_DIR/maggregatedataaccess_p.h \
    $$MASHUP_MASHUP_SRC_DIR/mmashupcanvas_p.h \
    $$MASHUP_MASHUP_SRC_DIR/mmashupcanvasview_p.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletsettings_p.h \
    $$MASHUP_MASHUP_SRC_DIR/mgconfdatastore_p.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletbutton.h \
    $$MASHUP_MASHUP_SRC_DIR/mapplethandle.h \
    $$MASHUP_MASHUP_SRC_DIR/mapplethandleview.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletinstancemanager.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletinstancedata.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletinventory.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletinventoryview.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletid.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletinstancemanagerdbusadaptor.h

SOURCES += $$MASHUP_MASHUP_SRC_DIR/mappletbutton.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletsharedmutex.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mapplethandle.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mapplethandleview.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletinstancemanager.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletinstancedata.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletinventory.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletinventoryview.cpp \
    $$MASHUP_MASHUP_SRC_DIR/maggregatedataaccess.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mmashupcanvas.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mmashupcanvasview.cpp \
    $$MASHUP_MASHUP_SRC_DIR/msubdatastore.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletloader.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletid.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletsettings.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletsettingsdialog.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mgconfdatastore.cpp \
    $$MASHUP_MASHUP_SRC_DIR/mappletinstancemanagerdbusadaptor.cpp

MASHUP_MODEL_HEADERS += \
    $$MASHUP_MASHUP_SRC_DIR/mmashupcanvasmodel.h \

MASHUP_PRIVATE_MODEL_HEADERS += \
    $$MASHUP_MASHUP_SRC_DIR/mapplethandlemodel.h \
    $$MASHUP_MASHUP_SRC_DIR/mappletinventorymodel.h

MODEL_HEADERS += $$MASHUP_MODEL_HEADERS $$MASHUP_PRIVATE_MODEL_HEADERS

PUBLIC_HEADERS += $$MASHUP_MODEL_HEADERS
PRIVATE_HEADERS += $$MASHUP_PRIVATE_MODEL_HEADERS
