###############################################################################
# mvideowidget, Separated to video folder.
###############################################################################
VIDEO_SRC_DIR = ./video
INCLUDEPATH +=./video

contains( DEFINES, HAVE_GSTREAMER) {
    MODEL_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidgetmodel.h

    PUBLIC_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidget.h \
        $$VIDEO_SRC_DIR/mvideo.h \
        $$MODEL_HEADERS \

    SOURCES += \
        $$VIDEO_SRC_DIR/mvideowidget.cpp \

}

