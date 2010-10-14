###############################################################################
# Animation/Core module
# This module contains core animation code.
###############################################################################

ANIMATIONS_SCENE_SRC_DIR=./animation/scene

PUBLIC_HEADERS += \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchslideanimation.h \

PRIVATE_HEADERS += \
    $$ANIMATIONS_SCENE_SRC_DIR/mbasicorientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/morientationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/morientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mcrossfadedorientationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mcrossfadedorientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/msnapshotitem.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchslideanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mbasicorientationanimation.h \

SOURCES += \
    $$ANIMATIONS_SCENE_SRC_DIR/mbasicorientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/morientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mcrossfadedorientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/msnapshotitem.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchslideanimation.cpp \
