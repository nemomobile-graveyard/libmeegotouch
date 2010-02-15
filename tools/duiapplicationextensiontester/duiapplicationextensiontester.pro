include(../../mkspecs/common.pri)

INCLUDEPATH += . \
    ../../src/include
DEPENDPATH += $$INCLUDEPATH
TEMPLATE = app
TARGET = duiapplicationextensiontester

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework dui
    }
    win32:LIBS += -L../../lib -ldui0
} else {
    LIBS += ../../lib/libdui.so
}

# Input
HEADERS += duiapplicationextensiontester.h

SOURCES += duiapplicationextensiontester.cpp

target.path = $$DUI_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
