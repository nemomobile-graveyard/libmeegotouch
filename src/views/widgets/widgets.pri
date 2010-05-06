WIDGETS_SRC_DIR=./widgets
INCLUDEPATH+=./widgets

PUBLIC_HEADERS += \
    $$WIDGETS_SRC_DIR/mapplicationmenubutton.h \
    $$WIDGETS_SRC_DIR/mlistindex.h \

MGEN_MODEL_HEADERS += \
    $$WIDGETS_SRC_DIR/mapplicationmenubuttonmodel.h \
    $$WIDGETS_SRC_DIR/mlistindexmodel.h \

PRIVATE_HEADERS += \
    $$MGEN_MODEL_HEADERS \
    $$WIDGETS_SRC_DIR/mcontainerheader_p.h \

SOURCES += \
    $$WIDGETS_SRC_DIR/mapplicationmenubutton.cpp \
    $$WIDGETS_SRC_DIR/mcontainerheader.cpp \
    $$WIDGETS_SRC_DIR/mlistindex.cpp \
