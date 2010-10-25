MROOT = ../..
include($$MROOT/mkspecs/common.pri)

MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include
MSFWINCLUDE = $$MROOT/servicefw/include
INCLUDEPATH += . \
    $$MSRCINCLUDE \
    $$MSRC
QMAKE_LIBDIR += $$MLIB

LIBS += $$mAddLibrary(meegotouchcore) \
        $$mAddLibrary(meegotouchviews) \
        $$mAddLibrary(meegotouchsettings) \
        $$mAddLibrary(meegotouchextensions)

TEMPLATE = app
TARGET = applicationextensiondemo
target.path = $$M_INSTALL_BIN
!win32:OBJECTS_DIR = .obj
MOC_DIR = ./.moc
DEPENDPATH += $$INCLUDEPATH
HEADERS += main.h demoapplicationextensioninterface.h
SOURCES += main.cpp
INSTALLS += target
