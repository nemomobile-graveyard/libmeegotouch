include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/include
DEPENDPATH += $$MSRCDIR/include
TARGET = mt_mlabel

SOURCES += mt_mlabel.cpp
HEADERS += mt_mlabel.h
CONFIG += release

LIBS += $$mAddLibrary(meegotouchviews)
