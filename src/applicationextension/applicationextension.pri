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
    $$EXTENSION_DIR/duiapplicationextensionarea_p.h

SOURCES += $$EXTENSION_DIR/duiextensionarea.cpp \
    $$EXTENSION_DIR/duiapplicationextensionarea.cpp \
    $$EXTENSION_DIR/duiapplicationextensionareaview.cpp \
    $$EXTENSION_DIR/duiextensionareaview.cpp \
    $$EXTENSION_DIR/duiextensionareamodel.cpp

WIDGET_MODEL_HEADERS += $$EXTENSION_DIR/duiextensionareamodel.h \
    $$EXTENSION_DIR/duiapplicationextensionareamodel.h

