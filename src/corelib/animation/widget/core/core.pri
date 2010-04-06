###############################################################################
# DuiAnimation/Widget/Core module
# This module contains widget core animation code.
###############################################################################

ANIMATIONS_WIDGET_CORE_SRC_DIR=./animation/widget/core

PUBLIC_HEADERS += \
    $$ANIMATIONS_WIDGET_CORE_SRC_DIR/duiabstractwidgetanimation.h \

PRIVATE_HEADERS += \
    $$ANIMATIONS_WIDGET_CORE_SRC_DIR/duiabstractwidgetanimation_p.h \

SOURCES += \
    $$ANIMATIONS_WIDGET_CORE_SRC_DIR/duiabstractwidgetanimation.cpp \
