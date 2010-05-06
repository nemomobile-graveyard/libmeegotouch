include(../mashup.pri)

MASHUP_APPLETINTERFACE_SRC_DIR = $$MASHUP_SRC_DIR/appletinterface
INCLUDEPATH += $$MASHUP_SRC_DIR/appletinterface

PUBLIC_HEADERS +=  $$MASHUP_APPLETINTERFACE_SRC_DIR/mappletinterface.h \
    $$MASHUP_APPLETINTERFACE_SRC_DIR/mappletmetadata.h

SOURCES +=  $$MASHUP_APPLETINTERFACE_SRC_DIR/mappletmetadata.cpp
