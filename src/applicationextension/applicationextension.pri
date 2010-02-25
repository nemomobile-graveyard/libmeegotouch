EXTENSION_DIR=./applicationextension
INCLUDEPATH+=./applicationextension \
    $$GEN_DIR

HEADERS += \
    $$EXTENSION_DIR/duiextensionarea.h \
    $$EXTENSION_DIR/duiextensionarea_p.h \
    $$EXTENSION_DIR/duiextensionareamodel.h \
    $$EXTENSION_DIR/duiextensionareaview.h \
    $$EXTENSION_DIR/duiextensionareaview_p.h \
    $$EXTENSION_DIR/duiapplicationextensionareamodel.h \
    $$EXTENSION_DIR/duiapplicationextensionareaview.h \
    $$EXTENSION_DIR/duiapplicationextensionareaview_p.h \
    $$EXTENSION_DIR/duiapplicationextensionarea.h \
    $$EXTENSION_DIR/duiapplicationextensionarea_p.h \
    $$EXTENSION_DIR/duiapplicationextensionmanager.h \
    $$EXTENSION_DIR/duiapplicationextensioninterface.h \
    $$EXTENSION_DIR/duiapplicationextensionloader.h \
    $$EXTENSION_DIR/duiapplicationextensionmetadata.h \
    $$EXTENSION_DIR/duiextensionrunner.h \
    $$EXTENSION_DIR/duiextensionhandlemodel.h \
    $$EXTENSION_DIR/duiextensionhandle.h \
    $$EXTENSION_DIR/duiextensionhandle_p.h \
    $$EXTENSION_DIR/duiextensionhandleview.h \
    $$EXTENSION_DIR/duiextensionhandleview_p.h \
    $$EXTENSION_DIR/duiapplicationextensionhandleview.h \
    $$EXTENSION_DIR/duiapplicationextensionhandleview_p.h

SOURCES += $$EXTENSION_DIR/duiextensionarea.cpp \
    $$EXTENSION_DIR/duiapplicationextensionarea.cpp \
    $$EXTENSION_DIR/duiapplicationextensionareaview.cpp \
    $$EXTENSION_DIR/duiextensionareaview.cpp \
    $$EXTENSION_DIR/duiextensionareamodel.cpp \
    $$EXTENSION_DIR/duiapplicationextensionmanager.cpp \
    $$EXTENSION_DIR/duiapplicationextensionloader.cpp \
    $$EXTENSION_DIR/duiapplicationextensionmetadata.cpp \
    $$EXTENSION_DIR/duiextensionrunner.cpp \
    $$EXTENSION_DIR/duiextensionhandle.cpp \
    $$EXTENSION_DIR/duiextensionhandleview.cpp \
    $$EXTENSION_DIR/duiapplicationextensionhandleview.cpp

WIDGET_MODEL_HEADERS += $$EXTENSION_DIR/duiextensionareamodel.h \
    $$EXTENSION_DIR/duiapplicationextensionareamodel.h \
    $$EXTENSION_DIR/duiextensionhandlemodel.h
