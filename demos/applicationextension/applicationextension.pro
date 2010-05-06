MROOT = ../..
include($$MROOT/mkspecs/common.pri)

MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include
MSFWINCLUDE = $$MROOT/servicefw/include
INCLUDEPATH += . \
    $$MSRCINCLUDE \
    $$MSRC
QMAKE_LIBDIR += $$MLIB
win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework \
            m
    }
    win32:LIBS += -lmeegotouchcore0
}
else:LIBS += \
    -lmeegotouchcore \
    -lmeegotouchviews \
    -lmeegotouchsettings \
    -lmeegotouchextensions \

TEMPLATE = app
TARGET = applicationextensiondemo
target.path = $$M_INSTALL_BIN
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc
DEPENDPATH += $$INCLUDEPATH
HEADERS += main.h demoapplicationextensioninterface.h
SOURCES += main.cpp
INSTALLS += target
