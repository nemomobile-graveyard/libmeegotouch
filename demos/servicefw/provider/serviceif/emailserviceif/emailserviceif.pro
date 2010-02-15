DUIROOT = ../../../../..
include($$DUIROOT/mkspecs/common.pri)

DUILIB = $$DUIROOT/lib
DUISIF = $$DUIROOT/demos/servicefw
DUISFW = $$DUIROOT/duiservicemapper
DUISIFLIB = $$DUISIF/lib
DUISIFINCLUDE = $$DUISIF/include
DUISFWINCLUDE = $$DUIROOT/src/servicefwif

INCLUDEPATH += \
    $$DUISFWINCLUDE \
    $$DUISFWINCLUDE/include \
    $$DUISIFINCLUDE \
    $$DUIROOT/src/include \

DEPENTPATH += $$INCLUDEPATH
TARGET = emailservice
DESTDIR = $$DUISIFLIB
TEMPLATE = lib
SOURCES += \
    emailserviceifproxy.cpp \
    emailserviceif.cpp \

HEADERS += \
    emailserviceifproxy.h \
    emailserviceif.h \

QT += dbus

target.path += $$[QT_INSTALL_LIBS]

headers.target = .dummy
headers.commands = touch $$headers.target
headers.path = $$[QT_INSTALL_HEADERS]
headers.files = $$HEADERS

xml.target = .dummy
xml.commands = touch $$xml.target
xml.path = $$DUI_DBUS_INTERFACES_DIR
xml.files = com.nokia.EmailServiceIf.xml


INSTALLS += \
    target \
    headers \
    xml \

