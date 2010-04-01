include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = duinotificationtool
INCLUDEPATH += . \
    ../../src/include \
    ../../src/corelib/notification


DEPENDPATH += $$INCLUDEPATH
win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework dui
    }
    win32:LIBS += -lduicore0
} else {
    LIBS += -lduicore
}

SOURCES += duinotificationtool.cpp
target.path = $$DUI_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
