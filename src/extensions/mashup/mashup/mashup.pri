include(../mashup.pri)
MASHUP_MASHUP_SRC_DIR = $$MASHUP_SRC_DIR/mashup

PUBLIC_HEADERS += $$MASHUP_MASHUP_SRC_DIR/duiappletbutton.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletsharedmutex.h \
    $$MASHUP_MASHUP_SRC_DIR/duiapplethandle.h \
    $$MASHUP_MASHUP_SRC_DIR/duiapplethandleview.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinstancemanager.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinstancedata.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinventory.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinventoryview.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinventorymodel.h \
    $$MASHUP_MASHUP_SRC_DIR/duiaggregatedataaccess.h \
    $$MASHUP_MASHUP_SRC_DIR/duimashupcanvas.h \
    $$MASHUP_MASHUP_SRC_DIR/duimashupcanvasmodel.h \
    $$MASHUP_MASHUP_SRC_DIR/duimashupcanvasview.h \
    $$MASHUP_MASHUP_SRC_DIR/duisubdatastore.h \
    $$MASHUP_MASHUP_SRC_DIR/duiapplethandlemodel.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletloader.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletid.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletsettings.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletsettingsdialog.h \
    $$MASHUP_MASHUP_SRC_DIR/duigconfdatastore.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinstancemanagerdbusadaptor.h


PRIVATE_HEADERS += $$MASHUP_MASHUP_SRC_DIR/duiappletsharedmutex_p.h \
    $$MASHUP_MASHUP_SRC_DIR/duiapplethandle_p.h \
    $$MASHUP_MASHUP_SRC_DIR/duiapplethandleview_p.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinventoryview_p.h \
    $$MASHUP_MASHUP_SRC_DIR/duiaggregatedataaccess_p.h \
    $$MASHUP_MASHUP_SRC_DIR/duimashupcanvas_p.h \
    $$MASHUP_MASHUP_SRC_DIR/duimashupcanvasview_p.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletsettings_p.h \
    $$MASHUP_MASHUP_SRC_DIR/duigconfdatastore_p.h \


SOURCES += $$MASHUP_MASHUP_SRC_DIR/duiappletbutton.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletsharedmutex.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiapplethandle.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiapplethandleview.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinstancemanager.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinstancedata.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinventory.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinventoryview.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiaggregatedataaccess.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duimashupcanvas.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duimashupcanvasview.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duisubdatastore.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletloader.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletid.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletsettings.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletsettingsdialog.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duigconfdatastore.cpp \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinstancemanagerdbusadaptor.cpp

DUIGEN_MODEL_HEADERS += $$MASHUP_MASHUP_SRC_DIR/duiapplethandlemodel.h \
    $$MASHUP_MASHUP_SRC_DIR/duimashupcanvasmodel.h \
    $$MASHUP_MASHUP_SRC_DIR/duiappletinventorymodel.h

PUBLIC_HEADERS += $$DUIGEN_MODEL_HEADERS
