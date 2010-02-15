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
    win32:LIBS += -L../../lib \
        -ldui0
}
else:LIBS += ../../lib/libdui.so

TARGET = $$qtLibraryTarget(fakeinstallationsource)
DESTDIR = ../../lib
target.path += $$DUI_APPLET_INSTALLATION_SOURCES_DIR
INSTALLS += target \
    style \
    view_conf \
    svg \
    images

style.path = $$DUI_THEME_DIR/../fakeinstallationsource/themes/style
style.files = *.css
view_conf.path = $$DUI_THEME_DIR/../fakeinstallationsource/themes
view_conf.files = *.conf
svg.path = $$DUI_THEME_DIR/../fakeinstallationsource/themes/svg
svg.files = images/*.svg
images.path = $$DUI_THEME_DIR/../fakeinstallationsource/themes/images
images.files = images/*.png
