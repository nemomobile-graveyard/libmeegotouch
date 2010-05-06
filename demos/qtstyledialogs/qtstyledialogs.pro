MROOT = ../..
include($$MROOT/mkspecs/common.pri)
TEMPLATE = app
TARGET = qtstyledialogs
target.path = $$M_INSTALL_BIN
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp \
    MainWindow.cpp
HEADERS += MainWindow.h
FORMS += MainWindow.ui

# UI style definition
style_sheet.path = $$M_THEME_DIR/../qtstyledialogs/themes/style
style_sheet.files = style/qtstyledialogs.css
view_configuration.path = $$M_THEME_DIR/../qtstyledialogs/themes
view_configuration.files = style/qtstyledialogs.conf
style_images.path = $$M_THEME_DIR/../qtstyledialogs/themes/images
style_images.files = images/*.png \
    images/*.jpg
style_svg.path = $$M_THEME_DIR/../qtstyledialogs/themes/svg
style_svg.files = images/*.svg

desktop_entry.path = $$M_INSTALL_DATA/applications
desktop_entry.files = \
  qtstyledialogs.desktop \
  qtstyledialogs-windows-style.desktop

myname = com.nokia.qtstyledialogs

services.CONFIG += no_check_exist
services.target = $${myname}.service
services.commands = cat $$IN_PWD/$${myname}.service.in \
                 | sed -e "s:@M_INSTALL_BIN@:$${M_INSTALL_BIN}:g" \
                 > $${myname}.service
services.files = $$OUT_PWD/$${myname}.service
services.path = $$M_DBUS_SERVICES_DIR


DEFINES += IMAGES_DIR=\"\\\"$$style_images.path/\\\"\"

# Install instructions
INSTALLS += target \
    style_sheet \
    style_images \
    style_svg \
    desktop_entry \

!win32:!macx {
    INSTALLS += services
}

