###############################################################################
# duivideowidget, Separated to video folder.
###############################################################################
VIDEO_SRC_DIR = ./video

# REMOVE THIS WHEN ITS AVAILABLE
#QT -= opengl

contains( DEFINES, HAVE_GSTREAMER) {
    PUBLIC_HEADERS += \
        $$VIDEO_SRC_DIR/duivideowidget.h \
        $$VIDEO_SRC_DIR/duisink.h \
        $$VIDEO_SRC_DIR/duivideowidgetshaders.h \
        $$VIDEO_SRC_DIR/duivideowidgetview.h \
        $$VIDEO_SRC_DIR/duivideo.h \
        $$VIDEO_SRC_DIR/duivideowidgetmodel.h

    PRIVATE_HEADERS += \
        $$VIDEO_SRC_DIR/duigstvideo.h \
        $$VIDEO_SRC_DIR/duivideowidgetview_p.h

    SOURCES += \
        $$VIDEO_SRC_DIR/duivideowidget.cpp \
        $$VIDEO_SRC_DIR/duivideowidgetview.cpp \
        $$VIDEO_SRC_DIR/duigstvideo.cpp \
        $$VIDEO_SRC_DIR/duisink.c

    DUIGEN_MODEL_HEADERS += \
        $$VIDEO_SRC_DIR/duivideowidgetmodel.h

}

