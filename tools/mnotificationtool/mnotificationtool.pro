include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = mnotificationtool
INCLUDEPATH += . \
    ../../src/include \
    ../../src/corelib/notification


DEPENDPATH += $$INCLUDEPATH

LIBS += $$mAddLibrary(meegotouchcore)

SOURCES += mnotificationtool.cpp
target.path = $$M_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
QT += dbus
