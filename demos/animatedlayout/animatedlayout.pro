MROOT = ../..

include($$MROOT/mkspecs/common.pri)

MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include
INCLUDEPATH += \
    . \
    $$MSRCINCLUDE \
    $$MSRC

QMAKE_LIBDIR += \
    $$MLIB

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework meegotouchcore \
            -framework meegotouchviews
    }
    win32:LIBS += -lmeegotouch0
} else {
    LIBS += -lmeegotouchcore
}

TEMPLATE = app
TARGET = animatedlayout
target.path = $$M_INSTALL_BIN
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc
DEPENDPATH += $$INCLUDEPATH

SOURCES += animatedlayout.cpp
RESOURCES +=

CONFIG += qt

# AnimatedLayout UI style definition
style_sheet.path = $$M_THEME_DIR/../animatedlayout/themes/style
style_sheet.files = style/animatedlayout.css
view_configuration.path = $$M_THEME_DIR/../animatedlayout/themes
view_configuration.files = style/animatedlayout.conf
style_images.path = $$M_THEME_DIR/../animatedlayout/themes/images
style_images.files = images/*

# Install instructions
INSTALLS += \
    target \
    style_sheet \
    style_images \
    view_configuration \
