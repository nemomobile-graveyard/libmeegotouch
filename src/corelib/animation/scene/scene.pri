###############################################################################
# DuiAnimation/Core module
# This module contains core animation code.
###############################################################################

ANIMATIONS_SCENE_SRC_DIR=./animation/scene

PUBLIC_HEADERS += \
    $$ANIMATIONS_SCENE_SRC_DIR/morientationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mbasicorientationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mfliporientationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mscenewindowanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mnavigationbaranimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mnotificationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mscenefadeanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchanimation.h \

PRIVATE_HEADERS += \
    $$ANIMATIONS_SCENE_SRC_DIR/morientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mbasicorientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mfliporientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mscenewindowanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mnavigationbaranimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mnotificationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mscenefadeanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchanimation_p.h \

SOURCES += \
    $$ANIMATIONS_SCENE_SRC_DIR/morientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mbasicorientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mfliporientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mscenewindowanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mnavigationbaranimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mnotificationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mscenefadeanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/mpageswitchanimation.cpp \
