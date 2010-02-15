include(../mashup.pri)

MASHUP_APPLETINSTALLATION_SRC_DIR = $$MASHUP_SRC_DIR/appletinstallation
INCLUDEPATH += $$MASHUP_SRC_DIR/appletinstallation

HEADERS += $$MASHUP_APPLETINSTALLATION_SRC_DIR/duiappletinstallationsourceinterface.h \
           $$MASHUP_APPLETINSTALLATION_SRC_DIR/duiappletinstantiator.h \
           $$MASHUP_APPLETINSTALLATION_SRC_DIR/duiappletinstantiator_p.h

SOURCES += $$MASHUP_APPLETINSTALLATION_SRC_DIR/duiappletinstantiator.cpp
