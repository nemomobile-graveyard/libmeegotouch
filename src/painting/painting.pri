###############################################################################
# DuiPainting module
# This module contains all classes that exclusively deal with painting.  
############################################################################### 
PAINTING_SRC_DIR=./painting
INCLUDEPATH+=./painting
HEADERS += \
    $$PAINTING_SRC_DIR/duigles2renderer.h \
    $$PAINTING_SRC_DIR/duiglshader.h \
    $$PAINTING_SRC_DIR/duiglshaderprogram.h \
    $$PAINTING_SRC_DIR/duiglshaderuniform.h \
    $$PAINTING_SRC_DIR/duiscalableimage.h

SOURCES += \
    $$PAINTING_SRC_DIR/duigles2renderer.cpp \
    $$PAINTING_SRC_DIR/duiglshader.cpp \
    $$PAINTING_SRC_DIR/duiglshaderprogram.cpp \
    $$PAINTING_SRC_DIR/duiglshaderuniform.cpp \
    $$PAINTING_SRC_DIR/duiscalableimage.cpp

install_shader_source.path = $$DUI_SHADER_SOURCE_DIR
install_shader_source.files = \
    $$PAINTING_SRC_DIR/shaders/default.frag \
    $$PAINTING_SRC_DIR/shaders/default.vert \
    $$PAINTING_SRC_DIR/shaders/yuv1.frag \
    $$PAINTING_SRC_DIR/shaders/yuv3.frag

INSTALLS += install_shader_source
