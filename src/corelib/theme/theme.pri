###############################################################################
# Theme module
# This module contains all classes that handle theme support.
############################################################################### 
THEME_SRC_DIR=./theme
INCLUDEPATH+=./theme
PUBLIC_HEADERS += \
    $$THEME_SRC_DIR/mtheme.h \
    $$THEME_SRC_DIR/mdefaultfonts.h \
    $$THEME_SRC_DIR/mpalette.h \

PRIVATE_HEADERS += \
    $$THEME_SRC_DIR/mimagedirectory.h \
    $$THEME_SRC_DIR/mthemedaemonclient.h \
    $$THEME_SRC_DIR/mremotethemedaemon.h \
    $$THEME_SRC_DIR/mlocalthemedaemon.h \
    $$THEME_SRC_DIR/mlogicalvalues.h \
    $$THEME_SRC_DIR/mthemedaemon.h \
    $$THEME_SRC_DIR/imthemedaemon.h \
    $$THEME_SRC_DIR/mthemedaemonprotocol.h \
    $$THEME_SRC_DIR/mcommonpixmaps.h \
    $$THEME_SRC_DIR/mtheme_p.h \
    $$THEME_SRC_DIR/mpixmaphandle.h \


SOURCES += $$THEME_SRC_DIR/mtheme.cpp \
    $$THEME_SRC_DIR/mimagedirectory.cpp \
    $$THEME_SRC_DIR/mcommonpixmaps.cpp \
    $$THEME_SRC_DIR/mthemedaemonclient.cpp \
    $$THEME_SRC_DIR/mremotethemedaemon.cpp \
    $$THEME_SRC_DIR/mlocalthemedaemon.cpp \
    $$THEME_SRC_DIR/mdefaultfonts.cpp \
    $$THEME_SRC_DIR/mlogicalvalues.cpp \
    $$THEME_SRC_DIR/mpalette.cpp \
    $$THEME_SRC_DIR/mthemedaemon.cpp \
    $$THEME_SRC_DIR/mthemedaemonprotocol.cpp \
    $$THEME_SRC_DIR/mpixmaphandle.cpp \


