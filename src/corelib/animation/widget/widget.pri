###############################################################################
# DuiAnimation/Widget module
# This module contains widget animation code.
###############################################################################

include(core/core.pri)

ANIMATIONS_WIDGET_SRC_DIR=./animation/widget

#public
HEADERS += \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetfadeinanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetfadeoutanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetslideinanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetslideoutanimation.h \
    #$$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetzoominanimation.h \
    #$$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetzoomoutanimation.h \

#private
HEADERS += \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetfadeinanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetfadeoutanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetslideinanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetslideoutanimation_p.h \
    #$$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetzoominanimation_p.h \
    #$$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetzoomoutanimation_p.h \

SOURCES += \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetfadeinanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetfadeoutanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetslideinanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetslideoutanimation.cpp \
    #$$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetzoominanimation.cpp \
    #$$ANIMATIONS_WIDGET_SRC_DIR/duiwidgetzoomoutanimation.cpp \

