include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/include $$MSRCDIR/theme $$MSRCDIR/painting
DEPENDPATH += $$INCLUDEPATH
TARGET = pt_mscalableimage

SOURCES += pt_mscalableimage.cpp
HEADERS += pt_mscalableimage.h

exists($$[QT_INSTALL_LIBS]/libQtOpenGL.so) {
   QT += opengl
}

