include(../mashup.pri)

MASHUP_APPLETINSTALLATION_SRC_DIR = $$MASHUP_SRC_DIR/appletinstallation
INCLUDEPATH += $$MASHUP_SRC_DIR/appletinstallation

PUBLIC_HEADERS += \
    $$MASHUP_APPLETINSTALLATION_SRC_DIR/mappletinstallationsourceinterface.h \
    $$MASHUP_APPLETINSTALLATION_SRC_DIR/mappletinstantiator.h \


PRIVATE_HEADERS += \
    $$MASHUP_APPLETINSTALLATION_SRC_DIR/mappletinstantiator_p.h


SOURCES += $$MASHUP_APPLETINSTALLATION_SRC_DIR/mappletinstantiator.cpp
