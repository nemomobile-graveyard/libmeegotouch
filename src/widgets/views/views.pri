###############################################################################
# DuiWidget/Views module
# This module contains all classes implemeting widget views.
###############################################################################

WIDGETS_VIEWS_SRC_DIR=./widgets/views
INCLUDEPATH+=./widgets/views

PUBLIC_HEADERS += \
    $$WIDGETS_VIEWS_SRC_DIR/duiapplicationpageview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttoniconview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttonview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttonswitchview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicheckboxview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicomboboxview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicontainerview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duidialogview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duimodalscenewindowview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duidockwidgetview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiextendingbackgroundview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiescapebuttonpanelview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duihomebuttonpanelview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiimagewidgetview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiinfobannereventview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiinfobannerinformationview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duilabelview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duigriditemview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiapplicationmenuview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiapplicationmenubuttonview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duimenuobjectview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duimessageboxview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duinavigationbarview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duioverlayview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiobjectmenuview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duipannablewidgetview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duipopuplistview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duipositionindicatorview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiprogressindicatorbarview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiscenewindowview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duisliderview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duispinnerview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duitexteditview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duitoolbartabview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duitoolbarview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiseparatorview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicompleterview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duifastlistview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicontentitemview.h \


PRIVATE_HEADERS += \
    $$WIDGETS_VIEWS_SRC_DIR/duistylablewidgetview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duistatusbarview.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiextendingbackgroundview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duitexteditview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiapplicationmenuview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duiobjectmenuview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicompleterview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duisliderview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duispinnerview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duifastlistview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duicontentitemview_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttongrouplayoutpolicy_p.h \
    $$WIDGETS_VIEWS_SRC_DIR/duitoolbartabview_p.h \

SOURCES += \
    $$WIDGETS_VIEWS_SRC_DIR/duiapplicationpageview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttoniconview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttonview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttonswitchview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duicheckboxview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duicomboboxview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duicontainerview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duidialogview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duimodalscenewindowview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duidockwidgetview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiextendingbackgroundview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiescapebuttonpanelview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duihomebuttonpanelview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiimagewidgetview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiinfobannereventview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiinfobannerinformationview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duilabelview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duilabelview_simple.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duilabelview_rich.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duigriditemview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiapplicationmenuview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiapplicationmenubuttonview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duimenuobjectview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duimessageboxview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duinavigationbarview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duioverlayview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiobjectmenuview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duipannablewidgetview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duipopuplistview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duipositionindicatorview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiprogressindicatorbarview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiscenewindowview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duisliderview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duispinnerview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duistatusbarview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duitexteditview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duitoolbartabview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duitoolbarview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duiseparatorview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duistylablewidgetview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duicompleterview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duifastlistview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duifastlistview_p.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duicontentitemview.cpp \
    $$WIDGETS_VIEWS_SRC_DIR/duibuttongrouplayoutpolicy_p.cpp \
