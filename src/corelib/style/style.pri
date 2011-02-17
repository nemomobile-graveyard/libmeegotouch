###############################################################################
# Style module
# This module contains all classes that handle style and CSS support.
###############################################################################
STYLE_SRC_DIR=./style
INCLUDEPATH+=./style

PUBLIC_HEADERS += \
    $$STYLE_SRC_DIR/mstylecreator.h

PRIVATE_HEADERS += \
    $$STYLE_SRC_DIR/mstyle_p.h \
    $$STYLE_SRC_DIR/mstylesheet_p.h \
    $$STYLE_SRC_DIR/mstylesheetparser.h \
    $$STYLE_SRC_DIR/mstylesheetselector.h \
    $$STYLE_SRC_DIR/mstylesheetattribute.h \
    $$STYLE_SRC_DIR/mstylesheet.h \
    $$STYLE_SRC_DIR/muniquestringcache.h \
    $$STYLE_SRC_DIR/mstylesheetparser_p.h \
    $$STYLE_SRC_DIR/mstylesheetselectortree.h \

STYLE_HEADERS += \
    $$STYLE_SRC_DIR/mstyle.h \
    $$STYLE_SRC_DIR/mwidgetstyle.h \
    $$STYLE_SRC_DIR/mabstractlayoutpolicystyle.h \
    $$STYLE_SRC_DIR/mabstractwidgetanimationstyle.h \
    $$STYLE_SRC_DIR/manimationstyle.h \
    $$STYLE_SRC_DIR/mscenewindowanimationstyle.h \
    $$STYLE_SRC_DIR/mbasiclayoutanimationstyle.h \
    $$STYLE_SRC_DIR/mcrossfadedorientationanimationstyle.h \
    $$STYLE_SRC_DIR/mdialoganimationstyle.h \
    $$STYLE_SRC_DIR/mapplicationmenuanimationstyle.h \
    $$STYLE_SRC_DIR/mgroupanimationstyle.h \
    $$STYLE_SRC_DIR/mlayoutanimationstyle.h \
    $$STYLE_SRC_DIR/mwidgetfadeanimationstyle.h \
    $$STYLE_SRC_DIR/mwidgetmoveanimationstyle.h \
    $$STYLE_SRC_DIR/mwidgetslideanimationstyle.h \
    $$STYLE_SRC_DIR/mwidgetzoomanimationstyle.h \
    $$STYLE_SRC_DIR/mwidgetscalefadeanimationstyle.h \
    $$STYLE_SRC_DIR/mpageswitchanimationstyle.h \
    $$STYLE_SRC_DIR/mbasicorientationanimationstyle.h \
    $$STYLE_SRC_DIR/mscenemanagerstyle.h \
    $$STYLE_SRC_DIR/mpanrecognizerstyle.h \
    $$STYLE_SRC_DIR/mswiperecognizerstyle.h \
    $$STYLE_SRC_DIR/mtapandholdrecognizerstyle.h \
    $$STYLE_SRC_DIR/mapplicationwindowstyle.h \
    $$STYLE_SRC_DIR/mgraphicseffectstyle.h \
    $$STYLE_SRC_DIR/mwidgetrectangularclipanimationstyle.h \
    $$STYLE_SRC_DIR/mrelocatorstyle.h \
    $$STYLE_SRC_DIR/mwindowstyle.h \
    $$STYLE_SRC_DIR/mcontentfadeandslideanimationstyle.h \
    $$STYLE_SRC_DIR/mtapstatemachinestyle.h \

PUBLIC_HEADERS += \
    $$STYLE_HEADERS

SOURCES += \
    $$STYLE_SRC_DIR/mstyle.cpp \
    $$STYLE_SRC_DIR/mstylecreator.cpp \
    $$STYLE_SRC_DIR/mstylesheet.cpp \
    $$STYLE_SRC_DIR/mstylesheetparser.cpp \
    $$STYLE_SRC_DIR/mstylesheetselector.cpp \
    $$STYLE_SRC_DIR/mstylesheetattribute.cpp \
    $$STYLE_SRC_DIR/muniquestringcache.cpp \
    $$STYLE_SRC_DIR/mstylesheetselectortree.cpp \

