###############################################################################
# Help module
# This module contains all the classes needed to control the actual help
# application.
###############################################################################
HELP_SRC_DIR=./help
INCLUDEPATH+=./help

PUBLIC_HEADERS += \
    $$HELP_SRC_DIR/mhelpbutton.h \

PRIVATE_HEADERS += \
    $$HELP_SRC_DIR/mhelpbutton_p.h \
    $$HELP_SRC_DIR/muserguideif.h \
    $$HELP_SRC_DIR/muserguideifproxy.h \

SOURCES += $$HELP_SRC_DIR/muserguideif.cpp \
    $$HELP_SRC_DIR/muserguideifproxy.cpp \
    $$HELP_SRC_DIR/mhelpbutton.cpp \
