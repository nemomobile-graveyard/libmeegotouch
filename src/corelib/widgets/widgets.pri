###############################################################################
# Widget module
# This module contains all classes that represent widgets.
###############################################################################

WIDGETS_SRC_DIR=./widgets
INCLUDEPATH+=./widgets

include(core/core.pri)

PUBLIC_HEADERS += \
    $$WIDGETS_SRC_DIR/mapplicationpage.h \
    $$WIDGETS_SRC_DIR/mapplicationmenu.h \
    $$WIDGETS_SRC_DIR/mapplicationwindow.h \
    $$WIDGETS_SRC_DIR/mbutton.h \
    $$WIDGETS_SRC_DIR/mbuttongroup.h \
    $$WIDGETS_SRC_DIR/mcombobox.h \
    $$WIDGETS_SRC_DIR/mcontainer.h \
    $$WIDGETS_SRC_DIR/mdialog.h \
    $$WIDGETS_SRC_DIR/mmodalscenewindow.h \
    $$WIDGETS_SRC_DIR/mdockwidget.h \
    $$WIDGETS_SRC_DIR/mescapebuttonpanel.h \
    $$WIDGETS_SRC_DIR/mhomebuttonpanel.h \
    $$WIDGETS_SRC_DIR/mimagewidget.h \
    $$WIDGETS_SRC_DIR/minfobanner.h \
    $$WIDGETS_SRC_DIR/mlabel.h \
    $$WIDGETS_SRC_DIR/mlabelhighlighter.h \
    $$WIDGETS_SRC_DIR/mlist.h \
    $$WIDGETS_SRC_DIR/mgriditem.h \
    $$WIDGETS_SRC_DIR/mmessagebox.h \
    $$WIDGETS_SRC_DIR/mnavigationbar.h \
    $$WIDGETS_SRC_DIR/moverlay.h \
    $$WIDGETS_SRC_DIR/mpannablewidget.h \
    $$WIDGETS_SRC_DIR/mphysics2dpanning.h \
    $$WIDGETS_SRC_DIR/mpopuplist.h \
    $$WIDGETS_SRC_DIR/mpopuplist_p.h \
    $$WIDGETS_SRC_DIR/mpositionindicator.h \
    $$WIDGETS_SRC_DIR/mprogressindicator.h \
    $$WIDGETS_SRC_DIR/mscenewindow.h \
    $$WIDGETS_SRC_DIR/mseekbar.h \
    $$WIDGETS_SRC_DIR/mslider.h \
    $$WIDGETS_SRC_DIR/mstatusbar.h \
    $$WIDGETS_SRC_DIR/mstatusbar_p.h \
    $$WIDGETS_SRC_DIR/mtextedit.h \
    $$WIDGETS_SRC_DIR/mcompleter.h \
    $$WIDGETS_SRC_DIR/mcompleter_p.h \
    $$WIDGETS_SRC_DIR/mtoolbar.h \
    $$WIDGETS_SRC_DIR/mviewcreator.h \
    $$WIDGETS_SRC_DIR/mwindow.h \
    $$WIDGETS_SRC_DIR/mseparator.h \
    $$WIDGETS_SRC_DIR/mstylablewidget.h \
    $$WIDGETS_SRC_DIR/mpannableviewport.h \
    $$WIDGETS_SRC_DIR/mwidgetrecycler.h \
    $$WIDGETS_SRC_DIR/mabstractcellcreator.h \
    $$WIDGETS_SRC_DIR/mcontentitem.h \


MGEN_MODEL_HEADERS += \
    $$WIDGETS_SRC_DIR/mwidgetmodel.h \
    $$WIDGETS_SRC_DIR/mbuttonmodel.h \
    $$WIDGETS_SRC_DIR/mcomboboxmodel.h \
    $$WIDGETS_SRC_DIR/mlabelmodel.h \
    $$WIDGETS_SRC_DIR/mdialogmodel.h \
    $$WIDGETS_SRC_DIR/mmodalscenewindowmodel.h \
    $$WIDGETS_SRC_DIR/mdockwidgetmodel.h \
    $$WIDGETS_SRC_DIR/mescapebuttonpanelmodel.h \
    $$WIDGETS_SRC_DIR/mimagewidgetmodel.h \
    $$WIDGETS_SRC_DIR/minfobannermodel.h \
    $$WIDGETS_SRC_DIR/mapplicationmenumodel.h \
    $$WIDGETS_SRC_DIR/mmessageboxmodel.h \
    $$WIDGETS_SRC_DIR/mnavigationbarmodel.h \
    $$WIDGETS_SRC_DIR/mobjectmenumodel.h \
    $$WIDGETS_SRC_DIR/mapplicationpagemodel.h \
    $$WIDGETS_SRC_DIR/mscenewindowmodel.h \
    $$WIDGETS_SRC_DIR/mscenelayereffectmodel.h \
    $$WIDGETS_SRC_DIR/mlistmodel.h \
    $$WIDGETS_SRC_DIR/mgriditemmodel.h \
    $$WIDGETS_SRC_DIR/mpopuplistmodel.h \
    $$WIDGETS_SRC_DIR/mpositionindicatormodel.h \
    $$WIDGETS_SRC_DIR/mprogressindicatormodel.h \
    $$WIDGETS_SRC_DIR/mseparatormodel.h \
    $$WIDGETS_SRC_DIR/mslidermodel.h \
    $$WIDGETS_SRC_DIR/mseekbarmodel.h \
    $$WIDGETS_SRC_DIR/mtexteditmodel.h \
    $$WIDGETS_SRC_DIR/mpannablewidgetmodel.h \
    $$WIDGETS_SRC_DIR/mpannableviewportmodel.h \
    $$WIDGETS_SRC_DIR/mcontainermodel.h \
    $$WIDGETS_SRC_DIR/mcompletermodel.h \
    $$WIDGETS_SRC_DIR/mcontentitemmodel.h \

PUBLIC_HEADERS += \
    $$MGEN_MODEL_HEADERS \

PRIVATE_HEADERS += \
    $$WIDGETS_SRC_DIR/mobjectmenu.h \
    $$WIDGETS_SRC_DIR/mpannableviewportlayout.h \
    $$WIDGETS_SRC_DIR/mcontentitem_p.h \
    $$WIDGETS_SRC_DIR/mbuttongroup_p.h \

SOURCES += \
    $$WIDGETS_SRC_DIR/mwidgetmodel.cpp \
    $$WIDGETS_SRC_DIR/mapplicationpage.cpp \
    $$WIDGETS_SRC_DIR/mapplicationwindow.cpp \
    $$WIDGETS_SRC_DIR/mbutton.cpp \
    $$WIDGETS_SRC_DIR/mbuttongroup.cpp \
    $$WIDGETS_SRC_DIR/mcombobox.cpp \
    $$WIDGETS_SRC_DIR/mcontainer.cpp \
    $$WIDGETS_SRC_DIR/mdialog.cpp \
    $$WIDGETS_SRC_DIR/mdialogmodel.cpp \
    $$WIDGETS_SRC_DIR/mmodalscenewindow.cpp \
    $$WIDGETS_SRC_DIR/mdockwidget.cpp \
    $$WIDGETS_SRC_DIR/mescapebuttonpanel.cpp \
    $$WIDGETS_SRC_DIR/mhomebuttonpanel.cpp \
    $$WIDGETS_SRC_DIR/mimagewidget.cpp \
    $$WIDGETS_SRC_DIR/minfobanner.cpp \
    $$WIDGETS_SRC_DIR/mlabel.cpp \
    $$WIDGETS_SRC_DIR/mlabelmodel.cpp \
    $$WIDGETS_SRC_DIR/mlabelhighlighter.cpp \
    $$WIDGETS_SRC_DIR/mlist.cpp \
    $$WIDGETS_SRC_DIR/mgriditem.cpp \
    $$WIDGETS_SRC_DIR/minfobannermodel.cpp \
    $$WIDGETS_SRC_DIR/mapplicationmenu.cpp \
    $$WIDGETS_SRC_DIR/mmessagebox.cpp \
    $$WIDGETS_SRC_DIR/mnavigationbar.cpp \
    $$WIDGETS_SRC_DIR/mobjectmenu.cpp \
    $$WIDGETS_SRC_DIR/mobjectmenumodel.cpp \
    $$WIDGETS_SRC_DIR/moverlay.cpp \
    $$WIDGETS_SRC_DIR/mpannablewidget.cpp \
    $$WIDGETS_SRC_DIR/mpannableviewport.cpp \
    $$WIDGETS_SRC_DIR/mpannableviewportlayout.cpp \
    $$WIDGETS_SRC_DIR/mphysics2dpanning.cpp \
    $$WIDGETS_SRC_DIR/mpopuplist.cpp \
    $$WIDGETS_SRC_DIR/mpositionindicator.cpp \
    $$WIDGETS_SRC_DIR/mprogressindicator.cpp \
    $$WIDGETS_SRC_DIR/mscenewindow.cpp \
    $$WIDGETS_SRC_DIR/mseekbar.cpp \
    $$WIDGETS_SRC_DIR/mslider.cpp \
    $$WIDGETS_SRC_DIR/mslidermodel.cpp \
    $$WIDGETS_SRC_DIR/mstatusbar.cpp \
    $$WIDGETS_SRC_DIR/mtextedit.cpp \
    $$WIDGETS_SRC_DIR/mcompleter.cpp \
    $$WIDGETS_SRC_DIR/mtoolbar.cpp \
    $$WIDGETS_SRC_DIR/mviewcreator.cpp \
    $$WIDGETS_SRC_DIR/mwindow.cpp \
    $$WIDGETS_SRC_DIR/mseparator.cpp \
    $$WIDGETS_SRC_DIR/mstylablewidget.cpp \
    $$WIDGETS_SRC_DIR/mwidgetrecycler.cpp \
    $$WIDGETS_SRC_DIR/mabstractcellcreator.cpp \
    $$WIDGETS_SRC_DIR/mcontentitem.cpp \
    $$WIDGETS_SRC_DIR/mcontentitemmodel.cpp \
    $$WIDGETS_SRC_DIR/mlistmodel.cpp \
    $$WIDGETS_SRC_DIR/mpopuplistmodel.cpp \
