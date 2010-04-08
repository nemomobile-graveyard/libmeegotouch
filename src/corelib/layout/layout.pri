###############################################################################
# Layout module
# This module contains layout related code.
###############################################################################

LAYOUT_SRC_DIR=./layout
INCLUDEPATH+=./layout

PUBLIC_HEADERS += \
    $$LAYOUT_SRC_DIR/mlayoutanimation.h \
    $$LAYOUT_SRC_DIR/mabstractlayoutpolicy.h \
    $$LAYOUT_SRC_DIR/mbasiclayoutanimation.h \
    $$LAYOUT_SRC_DIR/mflowlayoutpolicy.h \
    $$LAYOUT_SRC_DIR/mfreestylelayoutpolicy.h \
    $$LAYOUT_SRC_DIR/mgridlayoutpolicy.h \
    $$LAYOUT_SRC_DIR/mlayout.h \
    $$LAYOUT_SRC_DIR/mlinearlayoutpolicy.h \

PRIVATE_HEADERS += \
    $$LAYOUT_SRC_DIR/mlayout_p.h \
    $$LAYOUT_SRC_DIR/mlayouthelper_p.h \

SOURCES += \
    $$LAYOUT_SRC_DIR/mlayoutanimation.cpp \
    $$LAYOUT_SRC_DIR/mabstractlayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/mabstractlayoutpolicy_p.cpp \
    $$LAYOUT_SRC_DIR/mbasiclayoutanimation.cpp \
    $$LAYOUT_SRC_DIR/mbasiclayoutanimation_p.cpp \
    $$LAYOUT_SRC_DIR/mflowlayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/mflowlayoutpolicy_p.cpp \
    $$LAYOUT_SRC_DIR/mfreestylelayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/mfreestylelayoutpolicy_p.cpp \
    $$LAYOUT_SRC_DIR/mgridlayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/mgridlayoutpolicy_p.cpp \
    $$LAYOUT_SRC_DIR/mitemstate_p.cpp \
    $$LAYOUT_SRC_DIR/mlayout.cpp \
    $$LAYOUT_SRC_DIR/mlayout_p.cpp \
    $$LAYOUT_SRC_DIR/mlinearlayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/mlinearlayoutpolicy_p.cpp
