include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/include
DEPENDPATH += $$MSRCDIR/include
TARGET = pt_mslider

SOURCES += pt_mslider.cpp
HEADERS += pt_mslider.h

LIBS += -lmeegotouchviews
