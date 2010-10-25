include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/include $$MSRCDIR/widgets/views
DEPENDPATH += $$MSRCDIR/include
TARGET = pt_mprogressindicator

SOURCES += pt_mprogressindicator.cpp
HEADERS += pt_mprogressindicator.h

LIBS += $$mAddLibrary(meegotouchviews)
