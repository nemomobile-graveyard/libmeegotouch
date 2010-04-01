###############################################################################
# DuiAnimation module
# This module contains all classes that handle animations.
###############################################################################
ANIMATION_SRC_DIR=./animation
INCLUDEPATH+=./animation ./animation/core ./animation/scene ./animation/widget ./animation/widget/core

include(core/core.pri)
include(scene/scene.pri)
include(widget/widget.pri)

#public
HEADERS += \
    $$ANIMATION_SRC_DIR/duiwidgetanimation.h

SOURCES += \
    $$ANIMATION_SRC_DIR/duiwidgetanimation.cpp
