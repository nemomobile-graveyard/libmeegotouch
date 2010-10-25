include( ../mkspecs/common.pri )

INCLUDEPATH += $$M_SOURCE_TREE/src/include

LIBS += $$mAddLibrary(meegotouchcore)

DEPENDPATH += .
INCLUDEPATH += .
