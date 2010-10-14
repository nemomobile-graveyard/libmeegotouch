WIDGETS_SRC_DIR=./widgets
INCLUDEPATH+=./widgets

PUBLIC_HEADERS += \
    $$WIDGETS_SRC_DIR/mapplicationmenubutton.h \
    $$WIDGETS_SRC_DIR/mlistindex.h \
    $$WIDGETS_SRC_DIR/mcomboboxbutton.h \
    $$WIDGETS_SRC_DIR/mlistindextooltip.h \

MODEL_HEADERS += \
    $$WIDGETS_SRC_DIR/mapplicationmenubuttonmodel.h \
    $$WIDGETS_SRC_DIR/mlistindexmodel.h \

PRIVATE_HEADERS += \
    $$MODEL_HEADERS \
    $$WIDGETS_SRC_DIR/mcontainerheader_p.h \
    $$WIDGETS_SRC_DIR/mbubbleitembackground_p.h \
    $$WIDGETS_SRC_DIR/mlistindextooltip_p.h \

SOURCES += \
    $$WIDGETS_SRC_DIR/mapplicationmenubutton.cpp \
    $$WIDGETS_SRC_DIR/mcontainerheader.cpp \
    $$WIDGETS_SRC_DIR/mlistindex.cpp \
    $$WIDGETS_SRC_DIR/mbubbleitembackground.cpp \
    $$WIDGETS_SRC_DIR/mcomboboxbutton.cpp \
    $$WIDGETS_SRC_DIR/mlistindextooltip.cpp \
