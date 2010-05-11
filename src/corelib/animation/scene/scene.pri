###############################################################################
# Animation/Core module
# This module contains core animation code.
###############################################################################

ANIMATIONS_SCENE_SRC_DIR=./animation/scene

PUBLIC_HEADERS += \
    $$ANIMATIONS_SCENE_SRC_DIR/mbasicorientationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchanimation.h \

PRIVATE_HEADERS += \
    $$ANIMATIONS_SCENE_SRC_DIR/mbasicorientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchanimation_p.h \

SOURCES += \
    $$ANIMATIONS_SCENE_SRC_DIR/mbasicorientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchanimation.cpp \
