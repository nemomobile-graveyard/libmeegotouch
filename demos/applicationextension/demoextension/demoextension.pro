MOC_DIR = .moc
DUIGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

DUIROOT = ../../..
DUILIB = $$DUIROOT/lib
DUISRC = $$DUIROOT/src
DUISRCINCLUDE = $$DUISRC/include

include($$DUIROOT/mkspecs/common.pri)

TEMPLATE = lib
CONFIG += plugin \
    gui \
    link_pkgconfig

INCLUDEPATH += $$DUISRCINCLUDE ../
HEADERS = demoextension.h
SOURCES = demoextension.cpp

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../../lib
        LIBS += -framework \
            dui
    }
    win32:LIBS += -L../../../lib \
        -ldui0
}
else:LIBS += ../../../lib/libdui.so

TARGET = $$qtLibraryTarget(demoextension)
DESTDIR = ../../../lib
target.path += $$DUI_APPLICATION_EXTENSION_DIR
INSTALLS += target desktop_entry

desktop_entry.path = $$DUI_APPLICATION_EXTENSION_DATA_DIR
desktop_entry.files = *.desktop

