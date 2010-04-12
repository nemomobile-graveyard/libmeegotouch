###############################################################################
# Animation/Widget module
# This module contains widget animation code.
###############################################################################

include(core/core.pri)

ANIMATIONS_WIDGET_SRC_DIR=./animation/widget

PUBLIC_HEADERS += \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeinanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeoutanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideinanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideoutanimation.h \
    #$$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoominanimation.h \
    #$$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoomoutanimation.h \

PRIVATE_HEADERS += \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeinanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeoutanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideinanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideoutanimation_p.h \
    #$$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoominanimation_p.h \
    #$$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoomoutanimation_p.h \

SOURCES += \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeinanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeoutanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideinanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideoutanimation.cpp \
    #$$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoominanimation.cpp \
    #$$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoomoutanimation.cpp \

