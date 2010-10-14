###############################################################################
# Graphics effects module. Separated to effects folder.
###############################################################################
EFFECTS_SRC_DIR= ./effects
INCLUDEPATH+=./effects ./effects/core

include(core/core.pri)

PUBLIC_HEADERS += \
        $$EFFECTS_SRC_DIR/mgraphicsclipeffect.h \

PRIVATE_HEADERS += \
        $$EFFECTS_SRC_DIR/mgraphicsclipeffect_p.h \

SOURCES += \
        $$EFFECTS_SRC_DIR/mgraphicsclipeffect.cpp \
