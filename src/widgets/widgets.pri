###############################################################################
# DuiWidget module
# This module contains all classes that represent widgets.
###############################################################################

WIDGETS_SRC_DIR=./widgets
INCLUDEPATH+=./widgets

duigen_model.name = duigenerator model
duigen_model.input = WIDGET_MODEL_HEADERS
duigen_model.depends = ../duigen/duigen
duigen_model.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
duigen_model.commands += ../duigen/duigen --model ${QMAKE_FILE_NAME} $$GEN_DIR/
duigen_model.clean += $$GEN_DIR/gen_*
duigen_model.CONFIG = target_predeps no_link
duigen_model.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += duigen_model

include(core/core.pri)
include(views/views.pri)

PUBLIC_HEADERS += \
    $$WIDGETS_SRC_DIR/duiapplicationpage.h \
    $$WIDGETS_SRC_DIR/duiapplicationmenu.h \
    $$WIDGETS_SRC_DIR/duiapplicationmenubutton.h \
    $$WIDGETS_SRC_DIR/duiapplicationwindow.h \
    $$WIDGETS_SRC_DIR/duibutton.h \
    $$WIDGETS_SRC_DIR/duibuttongroup.h \
    $$WIDGETS_SRC_DIR/duicombobox.h \
    $$WIDGETS_SRC_DIR/duicontainer.h \
    $$WIDGETS_SRC_DIR/duidialog.h \
    $$WIDGETS_SRC_DIR/duimodalscenewindow.h \
    $$WIDGETS_SRC_DIR/duidockwidget.h \
    $$WIDGETS_SRC_DIR/duiescapebuttonpanel.h \
    $$WIDGETS_SRC_DIR/duihomebuttonpanel.h \
    $$WIDGETS_SRC_DIR/duiimagewidget.h \
    $$WIDGETS_SRC_DIR/duiinfobanner.h \
    $$WIDGETS_SRC_DIR/duilabel.h \
    $$WIDGETS_SRC_DIR/duilabelhighlighter.h \
    $$WIDGETS_SRC_DIR/duilist.h \
    $$WIDGETS_SRC_DIR/duigriditem.h \
    $$WIDGETS_SRC_DIR/duimessagebox.h \
    $$WIDGETS_SRC_DIR/duinavigationbar.h \
    $$WIDGETS_SRC_DIR/duioverlay.h \
    $$WIDGETS_SRC_DIR/duipannablewidget.h \
    $$WIDGETS_SRC_DIR/duiphysics2dpanning.h \
    $$WIDGETS_SRC_DIR/duipopuplist.h \
    $$WIDGETS_SRC_DIR/duipopuplist_p.h \
    $$WIDGETS_SRC_DIR/duipositionindicator.h \
    $$WIDGETS_SRC_DIR/duiprogressindicator.h \
    $$WIDGETS_SRC_DIR/duiscenewindow.h \
    $$WIDGETS_SRC_DIR/duiseekbar.h \
    $$WIDGETS_SRC_DIR/duislider.h \
    $$WIDGETS_SRC_DIR/duitextedit.h \
    $$WIDGETS_SRC_DIR/duicompleter.h \
    $$WIDGETS_SRC_DIR/duicompleter_p.h \
    $$WIDGETS_SRC_DIR/duitoolbar.h \
    $$WIDGETS_SRC_DIR/duiviewcreator.h \
    $$WIDGETS_SRC_DIR/duiwindow.h \
    $$WIDGETS_SRC_DIR/duiseparator.h \
    $$WIDGETS_SRC_DIR/duistylablewidget.h \
    $$WIDGETS_SRC_DIR/duipannableviewport.h \
    $$WIDGETS_SRC_DIR/duiwidgetrecycler.h \
    $$WIDGETS_SRC_DIR/duiabstractcellcreator.h \
    $$WIDGETS_SRC_DIR/duicontentitem.h \


WIDGET_MODEL_HEADERS += \
    $$WIDGETS_SRC_DIR/duiwidgetmodel.h \
    $$WIDGETS_SRC_DIR/duibuttonmodel.h \
    $$WIDGETS_SRC_DIR/duicomboboxmodel.h \
    $$WIDGETS_SRC_DIR/duilabelmodel.h \
    $$WIDGETS_SRC_DIR/duidialogmodel.h \
    $$WIDGETS_SRC_DIR/duimodalscenewindowmodel.h \
    $$WIDGETS_SRC_DIR/duidockwidgetmodel.h \
    $$WIDGETS_SRC_DIR/duiescapebuttonpanelmodel.h \
    $$WIDGETS_SRC_DIR/duiimagewidgetmodel.h \
    $$WIDGETS_SRC_DIR/duiinfobannermodel.h \
    $$WIDGETS_SRC_DIR/duiapplicationmenumodel.h \
    $$WIDGETS_SRC_DIR/duiapplicationmenubuttonmodel.h \
    $$WIDGETS_SRC_DIR/duimessageboxmodel.h \
    $$WIDGETS_SRC_DIR/duinavigationbarmodel.h \
    $$WIDGETS_SRC_DIR/duiobjectmenumodel.h \
    $$WIDGETS_SRC_DIR/duiapplicationpagemodel.h \
    $$WIDGETS_SRC_DIR/duiscenewindowmodel.h \
    $$WIDGETS_SRC_DIR/duiscenelayereffectmodel.h \
    $$WIDGETS_SRC_DIR/duilistmodel.h \
    $$WIDGETS_SRC_DIR/duigriditemmodel.h \
    $$WIDGETS_SRC_DIR/duipopuplistmodel.h \
    $$WIDGETS_SRC_DIR/duipositionindicatormodel.h \
    $$WIDGETS_SRC_DIR/duiprogressindicatormodel.h \
    $$WIDGETS_SRC_DIR/duiseparatormodel.h \
    $$WIDGETS_SRC_DIR/duislidermodel.h \
    $$WIDGETS_SRC_DIR/duiseekbarmodel.h \
    $$WIDGETS_SRC_DIR/duitexteditmodel.h \
    $$WIDGETS_SRC_DIR/duipannablewidgetmodel.h \
    $$WIDGETS_SRC_DIR/duipannableviewportmodel.h \
    $$WIDGETS_SRC_DIR/duicontainermodel.h \
    $$WIDGETS_SRC_DIR/duicompletermodel.h \
    $$WIDGETS_SRC_DIR/duicontentitemmodel.h \


PRIVATE_HEADERS += \
    $$WIDGETS_SRC_DIR/duiobjectmenu.h \
    $$WIDGETS_SRC_DIR/duipannableviewportlayout.h \
    $$WIDGETS_SRC_DIR/duicontainerheader_p.h \
    $$WIDGETS_SRC_DIR/duicontentitem_p.h \
    $$WIDGETS_SRC_DIR/duibuttongroup_p.h \

HEADERS += \
    $$WIDGET_MODEL_HEADERS \

SOURCES += \
    $$WIDGETS_SRC_DIR/duiwidgetmodel.cpp \
    $$WIDGETS_SRC_DIR/duiapplicationpage.cpp \
    $$WIDGETS_SRC_DIR/duiapplicationwindow.cpp \
    $$WIDGETS_SRC_DIR/duibutton.cpp \
    $$WIDGETS_SRC_DIR/duibuttongroup.cpp \
    $$WIDGETS_SRC_DIR/duicombobox.cpp \
    $$WIDGETS_SRC_DIR/duicontainer.cpp \
    $$WIDGETS_SRC_DIR/duidialog.cpp \
    $$WIDGETS_SRC_DIR/duidialogmodel.cpp \
    $$WIDGETS_SRC_DIR/duimodalscenewindow.cpp \
    $$WIDGETS_SRC_DIR/duidockwidget.cpp \
    $$WIDGETS_SRC_DIR/duiescapebuttonpanel.cpp \
    $$WIDGETS_SRC_DIR/duihomebuttonpanel.cpp \
    $$WIDGETS_SRC_DIR/duiimagewidget.cpp \
    $$WIDGETS_SRC_DIR/duiinfobanner.cpp \
    $$WIDGETS_SRC_DIR/duilabel.cpp \
    $$WIDGETS_SRC_DIR/duilabelmodel.cpp \
    $$WIDGETS_SRC_DIR/duilabelhighlighter.cpp \
    $$WIDGETS_SRC_DIR/duilist.cpp \
    $$WIDGETS_SRC_DIR/duigriditem.cpp \
    $$WIDGETS_SRC_DIR/duiinfobannermodel.cpp \
    $$WIDGETS_SRC_DIR/duiapplicationmenu.cpp \
    $$WIDGETS_SRC_DIR/duiapplicationmenubutton.cpp \
    $$WIDGETS_SRC_DIR/duimessagebox.cpp \
    $$WIDGETS_SRC_DIR/duinavigationbar.cpp \
    $$WIDGETS_SRC_DIR/duiobjectmenu.cpp \
    $$WIDGETS_SRC_DIR/duiobjectmenumodel.cpp \
    $$WIDGETS_SRC_DIR/duioverlay.cpp \
    $$WIDGETS_SRC_DIR/duipannablewidget.cpp \
    $$WIDGETS_SRC_DIR/duipannableviewport.cpp \
    $$WIDGETS_SRC_DIR/duipannableviewportlayout.cpp \
    $$WIDGETS_SRC_DIR/duiphysics2dpanning.cpp \
    $$WIDGETS_SRC_DIR/duipopuplist.cpp \
    $$WIDGETS_SRC_DIR/duipositionindicator.cpp \
    $$WIDGETS_SRC_DIR/duiprogressindicator.cpp \
    $$WIDGETS_SRC_DIR/duiscenewindow.cpp \
    $$WIDGETS_SRC_DIR/duiseekbar.cpp \
    $$WIDGETS_SRC_DIR/duislider.cpp \
    $$WIDGETS_SRC_DIR/duislidermodel.cpp \
    $$WIDGETS_SRC_DIR/duitextedit.cpp \
    $$WIDGETS_SRC_DIR/duicompleter.cpp \
    $$WIDGETS_SRC_DIR/duitoolbar.cpp \
    $$WIDGETS_SRC_DIR/duiviewcreator.cpp \
    $$WIDGETS_SRC_DIR/duiwindow.cpp \
    $$WIDGETS_SRC_DIR/duiseparator.cpp \
    $$WIDGETS_SRC_DIR/duistylablewidget.cpp \
    $$WIDGETS_SRC_DIR/duiwidgetrecycler.cpp \
        $$WIDGETS_SRC_DIR/duicontainerheader.cpp \
    $$WIDGETS_SRC_DIR/duiabstractcellcreator.cpp \
    $$WIDGETS_SRC_DIR/duicontentitem.cpp \
    $$WIDGETS_SRC_DIR/duicontentitemmodel.cpp \
    $$WIDGETS_SRC_DIR/duilistmodel.cpp \
