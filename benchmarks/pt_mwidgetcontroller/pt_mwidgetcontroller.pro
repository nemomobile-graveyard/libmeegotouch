include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/include
DEPENDPATH += $$MSRCDIR/include
TARGET = pt_mwidgetcontroller

SOURCES += pt_mwidgetcontroller.cpp
HEADERS += pt_mwidgetcontroller.h

LIBS += $$mAddLibrary(meegotouchviews)
