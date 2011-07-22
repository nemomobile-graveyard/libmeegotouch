ANIMATIONS_SRC_DIR=./animations
INCLUDEPATH+=./widgets

PRIVATE_HEADERS += \
    $$ANIMATIONS_SRC_DIR/mwarpanimation.h \
    $$ANIMATIONS_SRC_DIR/mbasiclistiteminsertionanimation.h \
    $$ANIMATIONS_SRC_DIR/mbasiclistiteminsertionanimation_p.h \
    $$ANIMATIONS_SRC_DIR/mbasiclistitemdeletionanimation.h \
    $$ANIMATIONS_SRC_DIR/mbasiclistitemdeletionanimation_p.h \

SOURCES += \
    $$ANIMATIONS_SRC_DIR/mwarpanimation.cpp \
    $$ANIMATIONS_SRC_DIR/mbasiclistiteminsertionanimation.cpp \
    $$ANIMATIONS_SRC_DIR/mbasiclistitemdeletionanimation.cpp \
