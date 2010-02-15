include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = duinotificationtool
INCLUDEPATH += . \
    ../../src/include \
    ../../src/notification
DEPENDPATH += $$INCLUDEPATH
win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework dui
    }
    win32:LIBS += -L../../lib -ldui0
} else {
    LIBS += ../../lib/libdui.so
}
SOURCES += duinotificationtool.cpp
target.path = $$DUI_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
