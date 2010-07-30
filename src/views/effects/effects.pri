###############################################################################
# Graphics effects module. Separated to effects folder.
###############################################################################
EFFECTS_SRC_DIR= ./effects
INCLUDEPATH+=./effects
PUBLIC_HEADERS += \
        $$EFFECTS_SRC_DIR/mgraphicshighlighteffect.h \

PRIVATE_HEADERS += \
        $$EFFECTS_SRC_DIR/mgraphicshighlighteffect_p.h \

SOURCES += \
        $$EFFECTS_SRC_DIR/mgraphicshighlighteffect.cpp \
