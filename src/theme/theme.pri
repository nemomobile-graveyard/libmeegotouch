###############################################################################
# DuiTheme module
# This module contains all classes that handle theme support.
############################################################################### 
THEME_SRC_DIR=./theme
INCLUDEPATH+=./theme
HEADERS += $$THEME_SRC_DIR/duitheme.h \
    $$THEME_SRC_DIR/duiimagedirectory.h \
    $$THEME_SRC_DIR/duicommonpixmaps.h \
    $$THEME_SRC_DIR/duithemedaemonclient.h \
    $$THEME_SRC_DIR/duiremotethemedaemon.h \
    $$THEME_SRC_DIR/duilocalthemedaemon.h \
    $$THEME_SRC_DIR/duidefaultfonts.h \
    $$THEME_SRC_DIR/duilogicalvalues.h \
    $$THEME_SRC_DIR/duipalette.h \
    $$THEME_SRC_DIR/duithemedaemon.h \
    $$THEME_SRC_DIR/iduithemedaemon.h \
    $$THEME_SRC_DIR/duithemedaemonprotocol.h \

SOURCES += $$THEME_SRC_DIR/duitheme.cpp \
    $$THEME_SRC_DIR/duiimagedirectory.cpp \
    $$THEME_SRC_DIR/duicommonpixmaps.cpp \
    $$THEME_SRC_DIR/duithemedaemonclient.cpp \
    $$THEME_SRC_DIR/duiremotethemedaemon.cpp \
    $$THEME_SRC_DIR/duilocalthemedaemon.cpp \
    $$THEME_SRC_DIR/duidefaultfonts.cpp \
    $$THEME_SRC_DIR/duilogicalvalues.cpp \
    $$THEME_SRC_DIR/duipalette.cpp \
    $$THEME_SRC_DIR/duithemedaemon.cpp \
    $$THEME_SRC_DIR/duithemedaemonprotocol.cpp \
