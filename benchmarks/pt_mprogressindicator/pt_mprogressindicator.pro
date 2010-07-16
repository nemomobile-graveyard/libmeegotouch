include(../common_top.pri)
INCLUDEPATH += ../../src/include ../../src/widgets/views
DEPENDPATH += ../../src/include
TARGET = pt_mprogressindicator

SOURCES += pt_mprogressindicator.cpp
HEADERS += pt_mprogressindicator.h

LIBS += -lmeegotouchviews
