include(../mashup.pri)

MASHUP_APPLETINTERFACE_SRC_DIR = $$MASHUP_SRC_DIR/appletinterface
INCLUDEPATH += $$MASHUP_SRC_DIR/appletinterface

HEADERS +=  $$MASHUP_APPLETINTERFACE_SRC_DIR/duiappletinterface.h \
    $$MASHUP_APPLETINTERFACE_SRC_DIR/duiappletmetadata.h

SOURCES +=  $$MASHUP_APPLETINTERFACE_SRC_DIR/duiappletmetadata.cpp
