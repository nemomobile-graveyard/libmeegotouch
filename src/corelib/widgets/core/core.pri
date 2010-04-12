###############################################################################
# Widget/Core module
# This module contains core widget code.
###############################################################################

WIDGETS_CORE_SRC_DIR=./widgets/core
INCLUDEPATH+=$$WIDGETS_CORE_SRC_DIR

PUBLIC_HEADERS += \
    $$WIDGETS_CORE_SRC_DIR/mwidget.h \
    $$WIDGETS_CORE_SRC_DIR/mwidgetview.h \
    $$WIDGETS_CORE_SRC_DIR/mwidgetcontroller.h \
    $$WIDGETS_CORE_SRC_DIR/mwidgetcreator.h \


SOURCES += \
    $$WIDGETS_CORE_SRC_DIR/mwidget.cpp \
    $$WIDGETS_CORE_SRC_DIR/mwidgetview.cpp \
    $$WIDGETS_CORE_SRC_DIR/mwidgetcontroller.cpp \
    $$WIDGETS_CORE_SRC_DIR/mwidgetcreator.cpp \
