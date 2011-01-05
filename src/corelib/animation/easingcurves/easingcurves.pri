###############################################################################
# Animation/Widget module
# This module contains widget animation code.
###############################################################################

EASING_CURVES_SRC_DIR = $$PWD

INCLUDEPATH += $$EASING_CURVES_SRC_DIR

PUBLIC_HEADERS += \
    $$EASING_CURVES_SRC_DIR/movershotbeziereasingcurve.h \

SOURCES += \
    $$EASING_CURVES_SRC_DIR/movershotbeziereasingcurve.cpp \
