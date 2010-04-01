DUIROOT = ../..
include($$DUIROOT/mkspecs/common.pri)

DUILIB = $$DUIROOT/lib
DUISRC = $$DUIROOT/src
DUISRCINCLUDE = $$DUISRC/include
DUISFWINCLUDE = $$DUIROOT/servicefw/include
INCLUDEPATH += . \
    $$DUISRCINCLUDE \
    $$DUISRC
QMAKE_LIBDIR += $$DUILIB
win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework \
            dui
    }
    win32:LIBS += -lduicore0
}
else:LIBS += \
    -lduicore \
    -lduiviews \
    -lduisettings \
    -lduiextensions \

TEMPLATE = app
TARGET = applicationextensiondemo
target.path = $$DUI_INSTALL_BIN
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc
DEPENDPATH += $$INCLUDEPATH
HEADERS += main.h demoapplicationextensioninterface.h
SOURCES += main.cpp
INSTALLS += target
