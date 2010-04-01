###############################################################################
# DuiTheme module
# This module contains all classes that handle style and CSS support.
############################################################################### 
SCENE_SRC_DIR=./scene
INCLUDEPATH+=./scene
HEADERS += $$SCENE_SRC_DIR/duiscene.h \
    $$SCENE_SRC_DIR/duiscenelayereffect.h \
    $$SCENE_SRC_DIR/duiscenelayereffect_p.h \
    $$SCENE_SRC_DIR/duiscenemanager.h \
    $$SCENE_SRC_DIR/duiscenemanager_p.h \
#    $$SCENE_SRC_DIR/duiscenewindowanimator.h \

SOURCES += $$SCENE_SRC_DIR/duiscene.cpp \
    $$SCENE_SRC_DIR/duiscenelayereffect.cpp \
    $$SCENE_SRC_DIR/duiscenemanager.cpp \
#    $$SCENE_SRC_DIR/duiscenewindowanimator.cpp \
