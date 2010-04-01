include(../common_top.pri)
INCLUDEPATH += ../../src/include
DEPENDPATH += ../../src/include
TARGET = mt_duilabel

SOURCES += mt_duilabel.cpp
HEADERS += mt_duilabel.h
CONFIG += release

LIBS += -lduiviews
