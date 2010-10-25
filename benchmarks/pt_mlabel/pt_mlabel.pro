include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/include
DEPENDPATH += $$MSRCDIR/include
TARGET = pt_mlabel

SOURCES += pt_mlabel.cpp
HEADERS += pt_mlabel.h

LIBS += $$mAddLibrary(meegotouchviews)
