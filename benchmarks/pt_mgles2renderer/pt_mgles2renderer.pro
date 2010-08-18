include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/include
DEPENDPATH += $$INCLUDEPATH
TARGET = pt_mgles2renderer

QT += opengl

SOURCES += pt_mgles2renderer.cpp

HEADERS += pt_mgles2renderer.h
