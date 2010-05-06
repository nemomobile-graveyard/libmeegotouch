MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

MROOT = ../../..
MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include

include($$MROOT/mkspecs/common.pri)

TEMPLATE = lib
CONFIG += plugin \
    gui \
    link_pkgconfig

INCLUDEPATH += $$MSRCINCLUDE ../
HEADERS = demoextension.h
SOURCES = demoextension.cpp

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../../lib
        LIBS += -framework \
            m
    }
    win32:LIBS += -lmeegotouchcore0
}
else:LIBS += -lmeegotouchcore

TARGET = $$qtLibraryTarget(applicationextensiondemo-demoextension)
DESTDIR = ../../../lib
target.path += $$M_APPLICATION_EXTENSION_DIR
INSTALLS += target desktop_entry

desktop_entry.path = $$M_APPLICATION_EXTENSION_DATA_DIR
desktop_entry.files = *.desktop
