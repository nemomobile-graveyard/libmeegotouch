###############################################################################
# duivideowidget, Separated to video folder.
###############################################################################
VIDEO_SRC_DIR = ./video

# REMOVE THIS WHEN ITS AVAILABLE
#QT -= opengl

#duigen_model.name = duigenerator model
#duigen_model.input = WIDGET_MODEL_HEADERS
#duigen_model.depends = ../duigen/duigen
#duigen_model.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
#duigen_model.commands += ../duigen/duigen --model ${QMAKE_FILE_NAME} $$GEN_DIR/
#duigen_model.clean += $$GEN_DIR/gen_*
#duigen_model.CONFIG = target_predeps no_link
#duigen_model.variable_out = GENERATED_SOURCES
#QMAKE_EXTRA_COMPILERS += duigen_model

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

    WIDGET_MODEL_HEADERS += \
        $$VIDEO_SRC_DIR/duivideowidgetmodel.h

}

