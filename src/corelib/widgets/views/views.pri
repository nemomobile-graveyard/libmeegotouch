###############################################################################
# Widget/Views module
# This module contains all classes implemeting widget views.
###############################################################################

WIDGETS_VIEWS_SRC_DIR=./widgets/views
INCLUDEPATH+=./widgets/views

PUBLIC_HEADERS += \
    $$WIDGETS_VIEWS_SRC_DIR/mscenewindowview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mlabelview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mpositionindicatorview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mapplicationmenubuttonview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mbuttonview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mimagewidgetview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mbuttoniconview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mbuttonswitchview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mcheckboxview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mcontainerview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mdockwidgetview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mcontentitemview.h \
    $$WIDGETS_VIEWS_SRC_DIR/mnavigationbarview.h \

PRIVATE_HEADERS += \

SOURCES += \
    $$WIDGETS_VIEWS_SRC_DIR/mscenewindowview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mlabelview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mlabelview_rich.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mlabelview_simple.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mpositionindicatorview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mapplicationmenubuttonview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mbuttonview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mimagewidgetview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mbuttoniconview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mbuttonswitchview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mcheckboxview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mcontainerview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mdockwidgetview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mcontentitemview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/mnavigationbarview.cpp \

