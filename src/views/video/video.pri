VIDEO_SRC_DIR = ./video
INCLUDEPATH +=./video

contains( DEFINES, HAVE_GSTREAMER) {
    PUBLIC_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidgetview.h \

    PRIVATE_HEADERS += \
        $$VIDEO_SRC_DIR/mvideowidgetview_p.h \
        $$VIDEO_SRC_DIR/msink.h \
        $$VIDEO_SRC_DIR/mgstvideo.h \

    SOURCES += \
        $$VIDEO_SRC_DIR/mvideowidgetview.cpp \
        $$VIDEO_SRC_DIR/msink.c \
        $$VIDEO_SRC_DIR/mgstvideo.cpp \
}
