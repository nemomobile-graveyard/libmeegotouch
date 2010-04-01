MOC_DIR = .moc
DUIGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

DUIROOT = ../..
DUILIB = $$DUIROOT/lib
DUISRC = $$DUIROOT/src
DUISRCINCLUDE = $$DUISRC/include

include($$DUIROOT/mkspecs/common.pri)

QT += dbus
TEMPLATE = lib
CONFIG += plugin \
    gui \
    link_pkgconfig

INCLUDEPATH += $$DUISRCINCLUDE
HEADERS = fakeinstallationsource.h
SOURCES = fakeinstallationsource.cpp

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework \
            dui
    }
    win32:LIBS += -lduicore0
}
else:LIBS += -lduicore

TARGET = $$qtLibraryTarget(appletinventory-fakeinstallationsource)
DESTDIR = ../../lib
target.path += $$DUI_APPLICATION_EXTENSION_DIR
INSTALLS += target \
    style \
    desktop_entry

desktop_entry.path = $$DUI_APPLICATION_EXTENSION_DATA_DIR
desktop_entry.files = *.desktop
