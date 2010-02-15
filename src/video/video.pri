###############################################################################
# duivideowidget, Separated to video folder.
############################################################################### 
VIDEO_SRC_DIR = ./video

# REMOVE THIS WHEN ITS AVAILABLE 
#QT -= opengl

contains( DEFINES, HAVE_GSTREAMER) {
    HEADERS += \
        $$VIDEO_SRC_DIR/duivideowidget.h \
        $$VIDEO_SRC_DIR/duisink.h \
        $$VIDEO_SRC_DIR/duivideowidgetshaders.h
 
    SOURCES += \
        $$VIDEO_SRC_DIR/duivideowidget.cpp \
        $$VIDEO_SRC_DIR/duisink.c
}
