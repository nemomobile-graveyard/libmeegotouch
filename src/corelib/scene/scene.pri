###############################################################################
# Scene module
# This module contains all classes that handle scene management
############################################################################### 
SCENE_SRC_DIR=./scene
INCLUDEPATH+=./scene

PUBLIC_HEADERS += $$SCENE_SRC_DIR/mscene.h \
    $$SCENE_SRC_DIR/mscenelayereffect.h \
    $$SCENE_SRC_DIR/mscenemanager.h \
    $$SCENE_SRC_DIR/msceneeventeater.h \

SOURCES += $$SCENE_SRC_DIR/mscene.cpp \
    $$SCENE_SRC_DIR/mscenelayereffect.cpp \
    $$SCENE_SRC_DIR/mscenemanager.cpp \
    $$SCENE_SRC_DIR/msceneeventeater.cpp

PRIVATE_HEADERS += \
    $$SCENE_SRC_DIR/mscenemanager_p.h \
    $$SCENE_SRC_DIR/mscenelayereffect_p.h \
