###############################################################################
# DuiLayout module
# This module contains layout related code.
###############################################################################

LAYOUT_SRC_DIR=./layout
INCLUDEPATH+=./layout

PUBLIC_HEADERS += \
    $$LAYOUT_SRC_DIR/duilayoutanimation.h \
    $$LAYOUT_SRC_DIR/duiabstractlayoutpolicy.h \
    $$LAYOUT_SRC_DIR/duibasiclayoutanimation.h \
    $$LAYOUT_SRC_DIR/duiflowlayoutpolicy.h \
    $$LAYOUT_SRC_DIR/duifreestylelayoutpolicy.h \
    $$LAYOUT_SRC_DIR/duigridlayoutpolicy.h \
    $$LAYOUT_SRC_DIR/duiitemstate.h \
    $$LAYOUT_SRC_DIR/duilayout.h \
    $$LAYOUT_SRC_DIR/duilinearlayoutpolicy.h \

PRIVATE_HEADERS += \
    $$LAYOUT_SRC_DIR/duilayout_p.h \
    $$LAYOUT_SRC_DIR/duilayouthelper_p.h \

SOURCES += \
    $$LAYOUT_SRC_DIR/duilayoutanimation.cpp \
    $$LAYOUT_SRC_DIR/duiabstractlayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/duiabstractlayoutpolicy_p.cpp \
    $$LAYOUT_SRC_DIR/duibasiclayoutanimation.cpp \
    $$LAYOUT_SRC_DIR/duibasiclayoutanimation_p.cpp \
    $$LAYOUT_SRC_DIR/duiflowlayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/duiflowlayoutpolicy_p.cpp \
    $$LAYOUT_SRC_DIR/duifreestylelayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/duifreestylelayoutpolicy_p.cpp \
    $$LAYOUT_SRC_DIR/duigridlayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/duigridlayoutpolicy_p.cpp \
    $$LAYOUT_SRC_DIR/duiitemstate.cpp \
    $$LAYOUT_SRC_DIR/duilayout.cpp \
    $$LAYOUT_SRC_DIR/duilayout_p.cpp \
    $$LAYOUT_SRC_DIR/duilinearlayoutpolicy.cpp \
    $$LAYOUT_SRC_DIR/duilinearlayoutpolicy_p.cpp
