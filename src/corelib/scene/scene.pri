###############################################################################
# DuiTheme module
# This module contains all classes that handle style and CSS support.
############################################################################### 
SCENE_SRC_DIR=./scene
INCLUDEPATH+=./scene

PUBLIC_HEADERS += $$SCENE_SRC_DIR/duiscene.h \
    $$SCENE_SRC_DIR/duiscenelayereffect.h \
    $$SCENE_SRC_DIR/duiscenelayereffect_p.h \
    $$SCENE_SRC_DIR/duiscenemanager.h \
    $$SCENE_SRC_DIR/duiscenemanager_p.h \

SOURCES += $$SCENE_SRC_DIR/duiscene.cpp \
    $$SCENE_SRC_DIR/duiscenelayereffect.cpp \
    $$SCENE_SRC_DIR/duiscenemanager.cpp \
