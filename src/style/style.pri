###############################################################################
# DuiTheme module
# This module contains all classes that handle style and CSS support.
###############################################################################
STYLE_SRC_DIR=./style
INCLUDEPATH+=./style

duigen_style.name = duigenerator style
duigen_style.input = WIDGET_STYLE_HEADERS
duigen_style.depends = ../duigen/duigen
duigen_style.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
duigen_style.commands += ../duigen/duigen --style ${QMAKE_FILE_NAME} $$GEN_DIR
duigen_style.clean += $$GEN_DIR/gen_*
duigen_style.CONFIG = target_predeps no_link
duigen_style.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += duigen_style

PUBLIC_HEADERS += \
    $$STYLE_SRC_DIR/duistylecreator.h \
    $$STYLE_SRC_DIR/duistylesheet.h \
    $$STYLE_SRC_DIR/duistylesheetparser.h \
    $$STYLE_SRC_DIR/duistylesheetselector.h \
    $$STYLE_SRC_DIR/duistylesheetattribute.h

PRIVATE_HEADERS += \
    $$STYLE_SRC_DIR/duistyle_p.h \
    $$STYLE_SRC_DIR/duistylesheet_p.h

WIDGET_STYLE_HEADERS += \
    $$STYLE_SRC_DIR/duiabstractlayoutpolicystyle.h \
    $$STYLE_SRC_DIR/duiabstractwidgetanimationstyle.h \
    $$STYLE_SRC_DIR/duianimationstyle.h \
    $$STYLE_SRC_DIR/duiappletinventorystyle.h \
    $$STYLE_SRC_DIR/duiscenewindowanimationstyle.h \
    $$STYLE_SRC_DIR/duiapplicationpagestyle.h \
    $$STYLE_SRC_DIR/duiapplethandlestyle.h \
    $$STYLE_SRC_DIR/duibasiclayoutanimationstyle.h \
    $$STYLE_SRC_DIR/duibuttonstyle.h \
    $$STYLE_SRC_DIR/duibuttoniconstyle.h \
    $$STYLE_SRC_DIR/duibuttonswitchstyle.h \
    $$STYLE_SRC_DIR/duicheckboxstyle.h \
    $$STYLE_SRC_DIR/duicomboboxstyle.h \
    $$STYLE_SRC_DIR/duicontainerstyle.h \
    $$STYLE_SRC_DIR/duimodalscenewindowstyle.h \
    $$STYLE_SRC_DIR/duidialogstyle.h \
    $$STYLE_SRC_DIR/duidockwidgetstyle.h \
    $$STYLE_SRC_DIR/duiextendingbackgroundstyle.h \
    $$STYLE_SRC_DIR/duiescapebuttonpanelstyle.h \
    $$STYLE_SRC_DIR/duiextensionhandlestyle.h \
    $$STYLE_SRC_DIR/duigriditemstyle.h \
    $$STYLE_SRC_DIR/duigroupanimationstyle.h \
    $$STYLE_SRC_DIR/duihomebuttonpanelstyle.h \
    $$STYLE_SRC_DIR/duiimagewidgetstyle.h \
    $$STYLE_SRC_DIR/duiinfobannerstyle.h \
    $$STYLE_SRC_DIR/duilabelstyle.h \
    $$STYLE_SRC_DIR/duilayoutanimationstyle.h \
    $$STYLE_SRC_DIR/duiliststyle.h \
    $$STYLE_SRC_DIR/duimashupcanvasstyle.h \
    $$STYLE_SRC_DIR/duiextensionareastyle.h \
    $$STYLE_SRC_DIR/duiapplicationextensionareastyle.h \
    $$STYLE_SRC_DIR/duiapplicationmenustyle.h \
    $$STYLE_SRC_DIR/duiapplicationmenubuttonstyle.h \
    $$STYLE_SRC_DIR/duimessageboxstyle.h \
    $$STYLE_SRC_DIR/duiscenelayereffectstyle.h \
    $$STYLE_SRC_DIR/duiscenelayereffectdimstyle.h \
    $$STYLE_SRC_DIR/duinavigationbarstyle.h \
    $$STYLE_SRC_DIR/duioverlaystyle.h \
    $$STYLE_SRC_DIR/duiobjectmenustyle.h \
    $$STYLE_SRC_DIR/duipannablewidgetstyle.h \
    $$STYLE_SRC_DIR/duipopupliststyle.h \
    $$STYLE_SRC_DIR/duipositionindicatorstyle.h \
    $$STYLE_SRC_DIR/duiprogressindicatorstyle.h \
    $$STYLE_SRC_DIR/duiscenewindowanimatorstyle.h \
    $$STYLE_SRC_DIR/duiscenewindowstyle.h \
    $$STYLE_SRC_DIR/duiseparatorstyle.h \
    $$STYLE_SRC_DIR/duisliderstyle.h \
    $$STYLE_SRC_DIR/duistyle.h \
    $$STYLE_SRC_DIR/duispinnerstyle.h \
    $$STYLE_SRC_DIR/duitexteditstyle.h \
    $$STYLE_SRC_DIR/duicompleterstyle.h \
    $$STYLE_SRC_DIR/duitoolbarstyle.h \
    $$STYLE_SRC_DIR/duiwidgetstyle.h \
    $$STYLE_SRC_DIR/duisettingslanguagesettingsfactorystyle.h \
    $$STYLE_SRC_DIR/duidevicestyle.h \
    $$STYLE_SRC_DIR/duiwidgetfadeinanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetfadeoutanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetslideinanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetslideoutanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetzoominanimationstyle.h \
    $$STYLE_SRC_DIR/duiwidgetzoomoutanimationstyle.h \
    $$STYLE_SRC_DIR/duicontainerheaderstyle.h \
    $$STYLE_SRC_DIR/duicontentitemstyle.h \
    $$STYLE_SRC_DIR/duipageswitchanimationstyle.h \
    $$STYLE_SRC_DIR/duibasicorientationanimationstyle.h \

HEADERS += \
    $$WIDGET_STYLE_HEADERS \

SOURCES += \
    $$STYLE_SRC_DIR/duistyle.cpp \
    $$STYLE_SRC_DIR/duistylecreator.cpp \
    $$STYLE_SRC_DIR/duistylesheet.cpp \
    $$STYLE_SRC_DIR/duistylesheetparser.cpp \
    $$STYLE_SRC_DIR/duistylesheetselector.cpp \
    $$STYLE_SRC_DIR/duistylesheetattribute.cpp
