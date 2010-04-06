###############################################################################
# DuiAnimation/Core module
# This module contains core animation code.
###############################################################################

ANIMATIONS_CORE_SRC_DIR=./animation/core

PUBLIC_HEADERS += \
    $$ANIMATIONS_CORE_SRC_DIR/duianimation.h \
    $$ANIMATIONS_CORE_SRC_DIR/duigroupanimation.h \
    $$ANIMATIONS_CORE_SRC_DIR/duianimationcreator.h \
    $$ANIMATIONS_CORE_SRC_DIR/duiparallelanimationgroup.h \

PRIVATE_HEADERS += \
    $$ANIMATIONS_CORE_SRC_DIR/duianimation_p.h \
    $$ANIMATIONS_CORE_SRC_DIR/duigroupanimation_p.h \
    $$ANIMATIONS_CORE_SRC_DIR/duiparallelanimationgroup_p.h \

SOURCES += \
    $$ANIMATIONS_CORE_SRC_DIR/duianimation.cpp \
    $$ANIMATIONS_CORE_SRC_DIR/duigroupanimation.cpp \
    $$ANIMATIONS_CORE_SRC_DIR/duianimationcreator.cpp \
    $$ANIMATIONS_CORE_SRC_DIR/duiparallelanimationgroup.cpp \
