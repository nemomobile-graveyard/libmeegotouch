###############################################################################
# DuiAnimation/Core module
# This module contains core animation code.
###############################################################################

ANIMATIONS_SCENE_SRC_DIR=./animation/scene

#public
HEADERS += \
    $$ANIMATIONS_SCENE_SRC_DIR/duiorientationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duibasicorientationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duifliporientationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duiscenewindowanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duinavigationbaranimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duinotificationanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duiscenefadeanimation.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duipageswitchanimation.h \

#private
HEADERS += \
    $$ANIMATIONS_SCENE_SRC_DIR/duiorientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duibasicorientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duifliporientationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duiscenewindowanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duinavigationbaranimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duinotificationanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duiscenefadeanimation_p.h \
    $$ANIMATIONS_SCENE_SRC_DIR/duipageswitchanimation_p.h \

SOURCES += \
    $$ANIMATIONS_SCENE_SRC_DIR/duiorientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/duibasicorientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/duifliporientationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/duiscenewindowanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/duinavigationbaranimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/duinotificationanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/duiscenefadeanimation.cpp \
    $$ANIMATIONS_SCENE_SRC_DIR/duipageswitchanimation.cpp \
