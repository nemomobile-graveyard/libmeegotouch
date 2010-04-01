###############################################################################
# DuiTheme module
# This module contains all classes that handle style and CSS support.
###############################################################################
STYLE_SRC_DIR=./style
INCLUDEPATH+=./style

PUBLIC_HEADERS += \
    $$STYLE_SRC_DIR/duistylecreator.h \
    $$STYLE_SRC_DIR/duistylesheet.h \
    $$STYLE_SRC_DIR/duistylesheetparser.h \
    $$STYLE_SRC_DIR/duistylesheetselector.h \
    $$STYLE_SRC_DIR/duistylesheetattribute.h

PRIVATE_HEADERS += \
    $$STYLE_SRC_DIR/duistyle_p.h \
    $$STYLE_SRC_DIR/duistylesheet_p.h \
    $$STYLE_SRC_DIR/duistylesheetselector_p.h

DUIGEN_STYLE_HEADERS += \
    $$STYLE_SRC_DIR/duistyle.h \
    $$STYLE_SRC_DIR/duiwidgetstyle.h \
    $$STYLE_SRC_DIR/duiabstractlayoutpolicystyle.h \
    $$STYLE_SRC_DIR/duiabstractwidgetanimationstyle.h \
    $$STYLE_SRC_DIR/duianimationstyle.h \
    $$STYLE_SRC_DIR/duiscenewindowanimationstyle.h \
    $$STYLE_SRC_DIR/duibasiclayoutanimationstyle.h \
    $$STYLE_SRC_DIR/duigroupanimationstyle.h \
    $$STYLE_SRC_DIR/duilayoutanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetfadeinanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetfadeoutanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetslideinanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetslideoutanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetzoominanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetzoomoutanimationstyle.h \
    $$STYLE_SRC_DIR/duipageswitchanimationstyle.h \
    $$STYLE_SRC_DIR/duibasicorientationanimationstyle.h \


HEADERS += \
    $$DUIGEN_STYLE_HEADERS \

SOURCES += \
    $$STYLE_SRC_DIR/duistyle.cpp \
    $$STYLE_SRC_DIR/duistylecreator.cpp \
    $$STYLE_SRC_DIR/duistylesheet.cpp \
    $$STYLE_SRC_DIR/duistylesheetparser.cpp \
    $$STYLE_SRC_DIR/duistylesheetselector.cpp \
    $$STYLE_SRC_DIR/duistylesheetattribute.cpp
