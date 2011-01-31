include( ../common.pri )

INCLUDEPATH += ../../../src/include
QMAKE_LIBDIR += ../../../lib/
LIBS += $$mAddLibrary(meegotouchcore)

DEPENDPATH += .
INCLUDEPATH += .

CONFIG += meegotouch-boostable

