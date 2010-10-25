MOC_DIR = .moc
MGEN_OUTDIR = .gen
!win32:OBJECTS_DIR = .obj

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

LIBS += $$mAddLibrary(meegotouchcore)

TARGET = $$qtLibraryTarget(applicationextensiondemo-demoextension2)
DESTDIR = ../../../lib
target.path += $$M_APPLICATION_EXTENSION_DIR
INSTALLS += target desktop_entry

desktop_entry.path = $$M_APPLICATION_EXTENSION_DATA_DIR
desktop_entry.files = *.desktop

