MROOT = ../../../..

include($$MROOT/mkspecs/common.pri)

MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include
MSFWINCLUDE = $$MROOT/servicefw/include

INCLUDEPATH += . \
    $$MSRCINCLUDE \
    $$MSRC \
    $$MSRC/mashup/appletinterface

QMAKE_LIBDIR += \
    $$MLIB

LIBS += $$mAddLibrary(meegotouchcore)

HEADERS       = galleryapplet.h \
                gallery.h \
                galleryview.h

SOURCES       = galleryapplet.cpp \
                gallery.cpp \
                galleryview.cpp

TEMPLATE = lib
# the z_ prefix is here to prevent this plugin to be the topmost plugin
TARGET        = $$qtLibraryTarget(galleryapplet)
DESTDIR       = ../../lib

target.path += $$M_APPLET_DIR

desktop_entry.path = $$M_APPLET_DATA_DIR
desktop_entry.files = galleryapplet.desktop

INSTALLS += \
    target \
    desktop_entry
