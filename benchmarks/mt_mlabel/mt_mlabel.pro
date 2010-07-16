include(../common_top.pri)
INCLUDEPATH += ../../src/include
DEPENDPATH += ../../src/include
TARGET = mt_mlabel

SOURCES += mt_mlabel.cpp
HEADERS += mt_mlabel.h
CONFIG += release

LIBS += -lmeegotouchviews
