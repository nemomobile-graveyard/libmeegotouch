include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = mnotificationtool
INCLUDEPATH += . \
    ../../src/include \
    ../../src/corelib/notification


DEPENDPATH += $$INCLUDEPATH
win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework m
    }
    win32:LIBS += -lmeegotouchcore0
} else {
    LIBS += -lmeegotouchcore
}

SOURCES += mnotificationtool.cpp
target.path = $$M_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
QT += dbus
