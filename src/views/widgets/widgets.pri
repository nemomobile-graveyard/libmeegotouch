WIDGETS_SRC_DIR=./widgets
INCLUDEPATH+=./widgets

PUBLIC_HEADERS += \
    $$WIDGETS_SRC_DIR/duiapplicationmenubutton.h \

DUIGEN_MODEL_HEADERS += \
    $$WIDGETS_SRC_DIR/duiapplicationmenubuttonmodel.h \

PRIVATE_HEADERS += \
    $$WIDGETS_SRC_DIR/duicontainerheader_p.h \

HEADERS += \
    $$DUIGEN_MODEL_HEADERS \

SOURCES += \
    $$WIDGETS_SRC_DIR/duiapplicationmenubutton.cpp \
    $$WIDGETS_SRC_DIR/duicontainerheader.cpp \
