WIDGETS_SRC_DIR=./widgets
INCLUDEPATH+=./widgets

PUBLIC_HEADERS += \
    $$WIDGETS_SRC_DIR/mapplicationmenubutton.h \

MGEN_MODEL_HEADERS += \
    $$WIDGETS_SRC_DIR/mapplicationmenubuttonmodel.h \

PRIVATE_HEADERS += \
    $$MGEN_MODEL_HEADERS \
    $$WIDGETS_SRC_DIR/mcontainerheader_p.h \

SOURCES += \
    $$WIDGETS_SRC_DIR/mapplicationmenubutton.cpp \
    $$WIDGETS_SRC_DIR/mcontainerheader.cpp \
