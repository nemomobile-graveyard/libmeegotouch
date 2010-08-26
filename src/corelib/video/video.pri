###############################################################################
# mvideowidget, Separated to video folder.
###############################################################################
VIDEO_SRC_DIR = ./video
INCLUDEPATH +=./video

contains( DEFINES, HAVE_GSTREAMER) {
    VIDEO_MODEL_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidgetmodel.h

    MODEL_HEADERS += $$VIDEO_MODEL_HEADERS

    PUBLIC_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidget.h \
        $$VIDEO_SRC_DIR/mvideo.h \
        $$VIDEO_MODEL_HEADERS \

    SOURCES += \
        $$VIDEO_SRC_DIR/mvideowidget.cpp \

}

