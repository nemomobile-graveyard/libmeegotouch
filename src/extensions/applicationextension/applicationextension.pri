EXTENSION_DIR=./applicationextension

PUBLIC_HEADERS += \
    $$EXTENSION_DIR/mextensionarea.h \
    $$EXTENSION_DIR/mextensionareaview.h \
    $$EXTENSION_DIR/mapplicationextensionareaview.h \
    $$EXTENSION_DIR/mapplicationextensionarea.h \
    $$EXTENSION_DIR/mapplicationextensioninterface.h \
    $$EXTENSION_DIR/mapplicationextensionmetadata.h \
    $$EXTENSION_DIR/mextensionrunner.h \

PRIVATE_HEADERS += \
    $$EXTENSION_DIR/mapplicationextensionhandleview.h \
    $$EXTENSION_DIR/mapplicationextensionloader.h \
    $$EXTENSION_DIR/mapplicationextensionmanager.h \
    $$EXTENSION_DIR/mextensionhandle.h \
    $$EXTENSION_DIR/mextensionhandleview.h \
    $$EXTENSION_DIR/mextensionarea_p.h \
    $$EXTENSION_DIR/mextensionareaview_p.h \
    $$EXTENSION_DIR/mapplicationextensionareaview_p.h \
    $$EXTENSION_DIR/mapplicationextensionarea_p.h \
    $$EXTENSION_DIR/mextensionhandle_p.h \
    $$EXTENSION_DIR/mextensionhandleview_p.h \
    $$EXTENSION_DIR/mapplicationextensionhandleview_p.h

SOURCES += $$EXTENSION_DIR/mextensionarea.cpp \
    $$EXTENSION_DIR/mapplicationextensionarea.cpp \
    $$EXTENSION_DIR/mapplicationextensionareaview.cpp \
    $$EXTENSION_DIR/mextensionareaview.cpp \
    $$EXTENSION_DIR/mextensionareamodel.cpp \
    $$EXTENSION_DIR/mapplicationextensionmanager.cpp \
    $$EXTENSION_DIR/mapplicationextensionloader.cpp \
    $$EXTENSION_DIR/mapplicationextensionmetadata.cpp \
    $$EXTENSION_DIR/mextensionrunner.cpp \
    $$EXTENSION_DIR/mextensionhandle.cpp \
    $$EXTENSION_DIR/mextensionhandleview.cpp \
    $$EXTENSION_DIR/mapplicationextensionhandleview.cpp


EXTENSION_MODEL_HEADERS += \
    $$EXTENSION_DIR/mextensionareamodel.h \
    $$EXTENSION_DIR/mapplicationextensionareamodel.h \

EXTENSION_PRIVATE_MODEL_HEADERS += \
    $$EXTENSION_DIR/mextensionhandlemodel.h

MODEL_HEADERS += $$EXTENSION_MODEL_HEADERS $$EXTENSION_PRIVATE_MODEL_HEADERS

PUBLIC_HEADERS += $$EXTENSION_MODEL_HEADERS
PRIVATE_HEADERS += $$EXTENSION_PRIVATE_MODEL_HEADERS
