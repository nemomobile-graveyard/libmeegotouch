MOC_DIR = .moc
MGEN_OUTDIR = .gen
!win32:OBJECTS_DIR = .obj

MROOT = ../..
MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include

include($$MROOT/mkspecs/common.pri)

QT += dbus
TEMPLATE = lib
CONFIG += plugin \
    gui \
    link_pkgconfig

INCLUDEPATH += $$MSRCINCLUDE
HEADERS = fakeinstallationsource.h
SOURCES = fakeinstallationsource.cpp

LIBS += $$mAddLibrary(meegotouchcore)

TARGET = $$qtLibraryTarget(appletinventory-fakeinstallationsource)
DESTDIR = ../../lib
target.path += $$M_APPLICATION_EXTENSION_DIR
INSTALLS += target \
    style \
    desktop_entry

desktop_entry.path = $$M_APPLICATION_EXTENSION_DATA_DIR
desktop_entry.files = *.desktop
