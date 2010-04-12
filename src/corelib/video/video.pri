###############################################################################
# mvideowidget, Separated to video folder.
###############################################################################
VIDEO_SRC_DIR = ./video

# REMOVE THIS WHEN ITS AVAILABLE
#QT -= opengl

contains( DEFINES, HAVE_GSTREAMER) {
    PUBLIC_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidget.h \
        $$VIDEO_SRC_DIR/msink.h \
        $$VIDEO_SRC_DIR/mvideowidgetshaders.h \
        $$VIDEO_SRC_DIR/mvideowidgetview.h \
        $$VIDEO_SRC_DIR/mvideo.h \
        $$VIDEO_SRC_DIR/mvideowidgetmodel.h

    PRIVATE_HEADERS += \
        $$VIDEO_SRC_DIR/mgstvideo.h \
        $$VIDEO_SRC_DIR/mvideowidgetview_p.h

    SOURCES += \
        $$VIDEO_SRC_DIR/mvideowidget.cpp \
        $$VIDEO_SRC_DIR/mvideowidgetview.cpp \
        $$VIDEO_SRC_DIR/mgstvideo.cpp \
        $$VIDEO_SRC_DIR/msink.c

    MGEN_MODEL_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidgetmodel.h

}

