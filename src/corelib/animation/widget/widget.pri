###############################################################################
# Animation/Widget module
# This module contains widget animation code.
###############################################################################

include(core/core.pri)

ANIMATIONS_WIDGET_SRC_DIR=./animation/widget

PUBLIC_HEADERS += \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetmoveanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoomanimation.h \

PRIVATE_HEADERS += \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetmoveanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoomanimation_p.h \

SOURCES += \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetmoveanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoomanimation.cpp \

