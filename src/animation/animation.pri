###############################################################################
# DuiAnimation module
# This module contains all classes that handle animations.
###############################################################################
ANIMATION_SRC_DIR=./animation
INCLUDEPATH+=./animation ./animation/core ./animation/particle ./animation/scene ./animation/widget ./animation/widget/core

include(core/core.pri)
include(scene/scene.pri)
include(particle/particle.pri)
include(widget/widget.pri)

#public
HEADERS += \
    $$ANIMATION_SRC_DIR/duiparticlecloud.h \
    $$ANIMATION_SRC_DIR/duiparticleexplosion.h \
    $$ANIMATION_SRC_DIR/duiparticlefountain.h \
    $$ANIMATION_SRC_DIR/duiwidgetanimation.h

SOURCES += \
    $$ANIMATION_SRC_DIR/duiparticlecloud.cpp \
    $$ANIMATION_SRC_DIR/duiparticleexplosion.cpp \
    $$ANIMATION_SRC_DIR/duiparticlefountain.cpp \
    $$ANIMATION_SRC_DIR/duiwidgetanimation.cpp
