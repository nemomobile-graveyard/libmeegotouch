###############################################################################
# DuiWidget/Core module
# This module contains core widget code.
###############################################################################

WIDGETS_CORE_SRC_DIR=./widgets/core
INCLUDEPATH+=$$WIDGETS_CORE_SRC_DIR

PUBLIC_HEADERS += \
    $$WIDGETS_CORE_SRC_DIR/duiwidget.h \
    $$WIDGETS_CORE_SRC_DIR/duiwidgetview.h \
    $$WIDGETS_CORE_SRC_DIR/duiwidgetcontroller.h \
    $$WIDGETS_CORE_SRC_DIR/duiwidgetcreator.h \


SOURCES += \
    $$WIDGETS_CORE_SRC_DIR/duiwidget.cpp \
    $$WIDGETS_CORE_SRC_DIR/duiwidgetview.cpp \
    $$WIDGETS_CORE_SRC_DIR/duiwidgetcontroller.cpp \
    $$WIDGETS_CORE_SRC_DIR/duiwidgetcreator.cpp \
