###############################################################################
# Effects/Core module
# This module contains core effects code.
###############################################################################

EFFECTS_CORE_SRC_DIR=./effects/core

PUBLIC_HEADERS += \
    $$EFFECTS_CORE_SRC_DIR/meffectcreator.h \
    $$EFFECTS_CORE_SRC_DIR/mgraphicseffect.h \

PRIVATE_HEADERS += \
    $$EFFECTS_CORE_SRC_DIR/mgraphicseffect_p.h \

SOURCES += \
    $$EFFECTS_CORE_SRC_DIR/meffectcreator.cpp \
    $$EFFECTS_CORE_SRC_DIR/mgraphicseffect.cpp \
