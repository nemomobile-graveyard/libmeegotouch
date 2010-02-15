include(../common_top.pri)
INCLUDEPATH += ../../src/include ../../src/theme ../../src/painting
DEPENDPATH += $$INCLUDEPATH
TARGET = pt_duiscalableimage

SOURCES += pt_duiscalableimage.cpp
HEADERS += pt_duiscalableimage.h

exists($$[QT_INSTALL_LIBS]/libQtOpenGL.so) {
   QT += opengl
}

