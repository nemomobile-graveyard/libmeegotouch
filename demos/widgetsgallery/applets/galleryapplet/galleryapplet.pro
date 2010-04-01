DUIROOT = ../../../..

include($$DUIROOT/mkspecs/common.pri)

DUILIB = $$DUIROOT/lib
DUISRC = $$DUIROOT/src
DUISRCINCLUDE = $$DUISRC/include
DUISFWINCLUDE = $$DUIROOT/servicefw/include

INCLUDEPATH += . \
    $$DUISRCINCLUDE \
    $$DUISRC \
    $$DUISRC/mashup/appletinterface

QMAKE_LIBDIR += \
    $$DUILIB \

win32|macx {
	macx {
	        QMAKE_LFLAGS += -F$$DUILIB
	        LIBS += -framework dui
	     }
        win32:LIBS += -lduicore0
} else {
    LIBS += -lduicore
}
					    

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

target.path += $$DUI_APPLET_DIR

desktop_entry.path = $$DUI_APPLET_DATA_DIR
desktop_entry.files = galleryapplet.desktop

INSTALLS += \
    target \
    desktop_entry
