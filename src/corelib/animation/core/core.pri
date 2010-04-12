###############################################################################
# Animation/Core module
# This module contains core animation code.
###############################################################################

ANIMATIONS_CORE_SRC_DIR=./animation/core

PUBLIC_HEADERS += \
    $$ANIMATIONS_CORE_SRC_DIR/manimation.h \
    $$ANIMATIONS_CORE_SRC_DIR/mgroupanimation.h \
    $$ANIMATIONS_CORE_SRC_DIR/manimationcreator.h \
    $$ANIMATIONS_CORE_SRC_DIR/mparallelanimationgroup.h \

PRIVATE_HEADERS += \
    $$ANIMATIONS_CORE_SRC_DIR/manimation_p.h \
    $$ANIMATIONS_CORE_SRC_DIR/mgroupanimation_p.h \
    $$ANIMATIONS_CORE_SRC_DIR/mparallelanimationgroup_p.h \

SOURCES += \
    $$ANIMATIONS_CORE_SRC_DIR/manimation.cpp \
    $$ANIMATIONS_CORE_SRC_DIR/mgroupanimation.cpp \
    $$ANIMATIONS_CORE_SRC_DIR/manimationcreator.cpp \
    $$ANIMATIONS_CORE_SRC_DIR/mparallelanimationgroup.cpp \
