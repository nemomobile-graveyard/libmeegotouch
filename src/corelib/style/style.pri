###############################################################################
# Style module
# This module contains all classes that handle style and CSS support.
###############################################################################
STYLE_SRC_DIR=./style
INCLUDEPATH+=./style

PUBLIC_HEADERS += \
    $$STYLE_SRC_DIR/mstylecreator.h \
    $$STYLE_SRC_DIR/mstylesheet.h \
    $$STYLE_SRC_DIR/mstylesheetparser.h \
    $$STYLE_SRC_DIR/mstylesheetselector.h \
    $$STYLE_SRC_DIR/mstylesheetattribute.h

PRIVATE_HEADERS += \
    $$STYLE_SRC_DIR/mstyle_p.h \
    $$STYLE_SRC_DIR/mstylesheet_p.h \
    $$STYLE_SRC_DIR/mstylesheetselector_p.h

MGEN_STYLE_HEADERS += \
    $$STYLE_SRC_DIR/mstyle.h \
    $$STYLE_SRC_DIR/mwidgetstyle.h \
    $$STYLE_SRC_DIR/mabstractlayoutpolicystyle.h \
    $$STYLE_SRC_DIR/mabstractwidgetanimationstyle.h \
    $$STYLE_SRC_DIR/manimationstyle.h \
    $$STYLE_SRC_DIR/mscenewindowanimationstyle.h \
    $$STYLE_SRC_DIR/mbasiclayoutanimationstyle.h \
    $$STYLE_SRC_DIR/mcrossfadedorientationanimationstyle.h \
    $$STYLE_SRC_DIR/mgroupanimationstyle.h \
    $$STYLE_SRC_DIR/mlayoutanimationstyle.h \
    $$STYLE_SRC_DIR/mwidgetfadeanimationstyle.h \
    $$STYLE_SRC_DIR/mwidgetslideanimationstyle.h \
    $$STYLE_SRC_DIR/mwidgetzoomanimationstyle.h \
    $$STYLE_SRC_DIR/mpageswitchanimationstyle.h \
    $$STYLE_SRC_DIR/mbasicorientationanimationstyle.h \


PUBLIC_HEADERS += \
    $$MGEN_STYLE_HEADERS \

SOURCES += \
    $$STYLE_SRC_DIR/mstyle.cpp \
    $$STYLE_SRC_DIR/mstylecreator.cpp \
    $$STYLE_SRC_DIR/mstylesheet.cpp \
    $$STYLE_SRC_DIR/mstylesheetparser.cpp \
    $$STYLE_SRC_DIR/mstylesheetselector.cpp \
    $$STYLE_SRC_DIR/mstylesheetattribute.cpp
