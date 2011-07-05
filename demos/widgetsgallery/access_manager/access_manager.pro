MROOT = ../../..
include($$MROOT/mkspecs/common.pri)
MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include

QMAKE_LIBDIR += $$MLIB

LIBS += $$mAddLibrary(meegotouchcore)

TEMPLATE = app
TARGET = widgetsgallery_accessmanager
target.path = $$M_INSTALL_BIN
DEPENDPATH += .
INCLUDEPATH += $$MSRCINCLUDE .
QT += dbus

# Input
HEADERS += accessmanageradaptor.h \
           accessmanagerimpl.h \
           loginsheet.h

SOURCES += accessmanageradaptor.cpp \
           main.cpp \
           accessmanagerimpl.cpp \
           loginsheet.cpp

myname = com.nokia.widgetsgallery.AccessManager
services.CONFIG += no_check_exist
services.target = $${myname}.service
services.commands = cat \
    $$IN_PWD/$${myname}.service.in \
    | \
    sed \
    -e \
    "s:@M_INSTALL_BIN@:$${M_INSTALL_BIN}:g" \
    > \
    $${myname}.service
services.files = $$OUT_PWD/$${myname}.service
services.path = $$M_DBUS_SERVICES_DIR

INSTALLS += target
!win32:!macx:INSTALLS += services
