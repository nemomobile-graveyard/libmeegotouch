###############################################################################
# Painting module
# This module contains all classes that exclusively deal with painting.  
############################################################################### 
PAINTING_SRC_DIR=./painting
INCLUDEPATH+=./painting

PUBLIC_HEADERS += \
    $$PAINTING_SRC_DIR/mgles2renderer.h \
    $$PAINTING_SRC_DIR/mbackgroundtiles.h \
    $$PAINTING_SRC_DIR/mscalableimage.h \
    $$PAINTING_SRC_DIR/mgraphicseffect.h \
    $$PAINTING_SRC_DIR/mgraphicsdimeffect.h \

PRIVATE_HEADERS += \
    $$PAINTING_SRC_DIR/mbackgroundtiles_p.h \
    $$PAINTING_SRC_DIR/mscalableimage_p.h \

SOURCES += \
    $$PAINTING_SRC_DIR/mgles2renderer.cpp \
    $$PAINTING_SRC_DIR/mbackgroundtiles.cpp \
    $$PAINTING_SRC_DIR/mscalableimage.cpp \
    $$PAINTING_SRC_DIR/mgraphicseffect.cpp \
    $$PAINTING_SRC_DIR/mgraphicsdimeffect.cpp \

install_shader_source.path = $$M_SHADER_SOURCE_DIR
install_shader_source.files = \
    $$PAINTING_SRC_DIR/shaders/default.frag \
    $$PAINTING_SRC_DIR/shaders/default.vert \
    $$PAINTING_SRC_DIR/shaders/yuv1.frag \
    $$PAINTING_SRC_DIR/shaders/yuv3.frag \
    $$PAINTING_SRC_DIR/shaders/dimeffect.frag \

INSTALLS += install_shader_source
