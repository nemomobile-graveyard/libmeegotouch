###############################################################################
# mvideowidget, Separated to video folder.
###############################################################################
VIDEO_SRC_DIR = ./video
INCLUDEPATH +=./video

contains( DEFINES, HAVE_GSTREAMER) {
    PUBLIC_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidget.h \
        $$VIDEO_SRC_DIR/mvideo.h \
        $$VIDEO_SRC_DIR/mvideowidgetmodel.h \

    SOURCES += \
        $$VIDEO_SRC_DIR/mvideowidget.cpp \

    MGEN_MODEL_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidgetmodel.h
}

