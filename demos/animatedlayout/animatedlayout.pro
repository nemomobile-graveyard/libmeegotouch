DUIROOT = ../..

include($$DUIROOT/mkspecs/common.pri)

DUILIB = $$DUIROOT/lib
DUISRC = $$DUIROOT/src
DUISRCINCLUDE = $$DUISRC/include
INCLUDEPATH += \
    . \
    $$DUISRCINCLUDE \
    $$DUISRC

QMAKE_LIBDIR += \
    $$DUILIB

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework dui
    }
    win32:LIBS += -L../../lib -ldui0
} else {
    LIBS += ../../lib/libdui.so
}

TEMPLATE = app
TARGET = animatedlayout
target.path = $$DUI_INSTALL_BIN
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc
DEPENDPATH += $$INCLUDEPATH

SOURCES += animatedlayout.cpp
RESOURCES +=

CONFIG += qt

# AnimatedLayout UI style definition
style_sheet.path = $$DUI_THEME_DIR/../animatedlayout/themes/style
style_sheet.files = style/animatedlayout.css
view_configuration.path = $$DUI_THEME_DIR/../animatedlayout/themes
view_configuration.files = style/animatedlayout.conf
style_images.path = $$DUI_THEME_DIR/../animatedlayout/themes/images
style_images.files = images/*

# Install instructions
INSTALLS += \
    target \
    style_sheet \
    style_images \
    view_configuration \
