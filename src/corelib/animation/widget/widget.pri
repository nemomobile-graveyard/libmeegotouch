###############################################################################
# Animation/Widget module
# This module contains widget animation code.
###############################################################################

include(core/core.pri)

ANIMATIONS_WIDGET_SRC_DIR=./animation/widget

PUBLIC_HEADERS += \
    $$ANIMATIONS_WIDGET_SRC_DIR/mapplicationmenuanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mdialoganimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetmoveanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoomanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetrectangularclipanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mcontentfadeandslideanimation.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetscalefadeanimation.h \

PRIVATE_HEADERS += \
    $$ANIMATIONS_WIDGET_SRC_DIR/mapplicationmenuanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mdialoganimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetmoveanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoomanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetrectangularclipanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mcontentfadeandslideanimation_p.h \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetscalefadeanimation_p.h \

SOURCES += \
    $$ANIMATIONS_WIDGET_SRC_DIR/mapplicationmenuanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mdialoganimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetfadeanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetmoveanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetslideanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetzoomanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetrectangularclipanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mcontentfadeandslideanimation.cpp \
    $$ANIMATIONS_WIDGET_SRC_DIR/mwidgetscalefadeanimation.cpp \
