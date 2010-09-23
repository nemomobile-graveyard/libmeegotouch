include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = mcssvalidator
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

SOURCES += mcssvalidator.cpp
target.path = $$M_INSTALL_BIN
INSTALLS += target
