MROOT = ../..
include($$MROOT/mkspecs/common.pri)
TEMPLATE = app
TARGET = qtstyleorientation
target.path = $$M_INSTALL_BIN
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp \
    mainwindow.cpp
HEADERS += mainwindow.h

desktop_entry.path = $$M_INSTALL_DATA/applications
desktop_entry.files = \
  qtstyleorientation.desktop

myname = com.nokia.qtstyleorientation

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
    desktop_entry

!win32:!macx {
    INSTALLS += services
}

RESOURCES += res.qrc
