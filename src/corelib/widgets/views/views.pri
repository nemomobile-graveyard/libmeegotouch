###############################################################################
# DuiWidget/Views module
# This module contains all classes implemeting widget views.
###############################################################################

WIDGETS_VIEWS_SRC_DIR=./widgets/views
INCLUDEPATH+=./widgets/views

PUBLIC_HEADERS += \
    $$WIDGETS_VIEWS_SRC_DIR/duiscenewindowview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duilabelview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duipositionindicatorview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiapplicationmenubuttonview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttonview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiimagewidgetview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttoniconview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttonswitchview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicheckboxview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicontainerview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duidockwidgetview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicontentitemview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duinavigationbarview.h \

PRIVATE_HEADERS += \

SOURCES += \
    $$WIDGETS_VIEWS_SRC_DIR/duiscenewindowview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duilabelview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duilabelview_rich.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duilabelview_simple.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duipositionindicatorview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiapplicationmenubuttonview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttonview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiimagewidgetview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttoniconview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttonswitchview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duicheckboxview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duicontainerview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duidockwidgetview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duicontentitemview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duinavigationbarview.cpp \

