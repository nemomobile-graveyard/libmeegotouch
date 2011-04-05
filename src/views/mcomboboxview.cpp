/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "mcomboboxview.h"
#include "mcomboboxview_p.h"
#include "mcombobox.h"

#include "mviewcreator.h"
#include "mpopuplist.h"
#include "mcomboboxbutton.h"
#include "mlabel.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mscenemanager.h"

MComboBoxViewPrivate::MComboBoxViewPrivate()
  : q_ptr(0),
    controller(0),
    button(0),
    popuplist(0),
    title(0),
    layout(0),
    separateTitle(0),
    integratedTitle(0),
    isPopupShowing(0)
{
}

MComboBoxViewPrivate::~MComboBoxViewPrivate()
{
    delete popuplist;
    delete button;
    delete title;
}

void MComboBoxViewPrivate::init()
{
    Q_Q(MComboBoxView);

    layout = new MLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(layout);

    QObject::connect(controller, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_itemModelCurrentIndexChanged(int)));
    QObject::connect(controller, SIGNAL(clicked()), q, SLOT(_q_showPopup()));
    QObject::connect(controller, SIGNAL(dismissed()), q ,SLOT(_q_dismissPopup()));
}

void MComboBoxViewPrivate::updateTitle(const QString& newTitle)
{
    buttonWidget()->setTitle(newTitle);
    if (title)
        title->setText(newTitle);
    if (popuplist)
        popuplist->setTitle(newTitle);
}

void MComboBoxViewPrivate::updateSubtitle(int currentIndex)
{
    if (currentIndex != -1)  {
        buttonWidget()->setSubtitle(controller->itemText(currentIndex));
    } else {
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: Default value for Popup List button sub-label when no item has been selected yet
        //% "Tap to Select"
        buttonWidget()->setSubtitle(qtTrId("qtn_comm_popup_default"));
    }
}

MComboBoxButton* MComboBoxViewPrivate::buttonWidget()
{
    if (!button) {
        button = new MComboBoxButton();
        QObject::connect(button, SIGNAL(clicked()), controller, SLOT(click()));
    }
    return button;
}

MLabel* MComboBoxViewPrivate::titleWidget()
{
    if (!title) {
        title = new MLabel();
        title->setTextElide(true);
    }
    return title;
}

MLinearLayoutPolicy* MComboBoxViewPrivate::separateTitlePolicy()
{
    if (!separateTitle) {
        separateTitle = new MLinearLayoutPolicy(layout, Qt::Vertical);
        separateTitle->addItem(titleWidget());
        separateTitle->addItem(buttonWidget());
    }
    return separateTitle;
}

MLinearLayoutPolicy* MComboBoxViewPrivate::integratedTitlePolicy()
{
    if (!integratedTitle) {
        integratedTitle = new MLinearLayoutPolicy(layout, Qt::Vertical);
        integratedTitle->addItem(buttonWidget());
    }
    return integratedTitle;
}

void MComboBoxViewPrivate::_q_itemModelCurrentIndexChanged(int currentIndex)
{
    updateSubtitle(currentIndex);
}

void MComboBoxViewPrivate::_q_showPopup()
{
    Q_Q(MComboBoxView);

    if (controller->count() == 0)
        return;

    if (!popuplist) {
        popuplist = new MPopupList();
        popuplist->setViewType(q->style()->popupListType());

        popuplist->setObjectName("MComboBoxPopupList");
        popuplist->setItemModel(controller->itemModel());
        popuplist->setSelectionModel(controller->selectionModel());
        popuplist->setTitle(controller->title());
        QObject::connect(popuplist, SIGNAL(clicked(QModelIndex)), controller, SLOT(_q_itemClicked(QModelIndex)));
        QObject::connect(popuplist, SIGNAL(appeared()), q, SLOT(_q_popuplistAppeared()));
        QObject::connect(popuplist, SIGNAL(disappeared()), q, SLOT(_q_popuplistDisappeared()));
        QObject::connect(popuplist, SIGNAL(rejected()), controller, SIGNAL(dismissed()));
    }

    popuplist->scrollTo(q->model()->selectionModel()->currentIndex());

    if (controller->sceneManager())
        controller->sceneManager()->appearSceneWindow(popuplist);
}

void MComboBoxViewPrivate::_q_dismissPopup()
{
    if (!popuplist)
        return;

    if (!isPopupShowing)
        return;

    QObject::disconnect(popuplist, SIGNAL(rejected()), controller, SIGNAL(dismissed()));
    popuplist->reject();
    QObject::connect(popuplist, SIGNAL(rejected()), controller, SIGNAL(dismissed()));
}

void MComboBoxViewPrivate::_q_popuplistAppeared()
{
    isPopupShowing = true;
}

void MComboBoxViewPrivate::_q_popuplistDisappeared()
{
    isPopupShowing = false;
}

MComboBoxView::MComboBoxView(MComboBox *controller) :
    MWidgetView(controller),
    d_ptr(new MComboBoxViewPrivate)
{
    Q_D(MComboBoxView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MComboBoxView::MComboBoxView(MComboBoxViewPrivate &dd, MComboBox *controller) :
    MWidgetView(controller),
    d_ptr(&dd)
{
    Q_D(MComboBoxView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MComboBoxView::~MComboBoxView()
{
    delete d_ptr;
}

void MComboBoxView::applyStyle()
{
    Q_D(MComboBoxView);
    MWidgetView::applyStyle();

    if (d->popuplist && d->popuplist->viewType() != style()->popupListType())
        d->popuplist->setViewType(style()->popupListType());

    d->buttonWidget()->setStyleName(style()->buttonStyleName());
    d->buttonWidget()->setTitleObjectName(style()->titleObjectName());
    d->buttonWidget()->setSubTitleObjectName(style()->subTitleObjectName());
    d->buttonWidget()->setIconObjectName(style()->iconObjectName());
    d->buttonWidget()->setIndicatorObjectName(style()->indicatorObjectName());
    d->buttonWidget()->setIndicatorImage(style()->indicatorImage());
    d->buttonWidget()->setProgressIndicatorObjectName(style()->progressIndicatorObjectName());

    d->layout->setPolicy(style()->separateTitle() && style()->hasTitle() ? d->separateTitlePolicy() : d->integratedTitlePolicy());
    d->buttonWidget()->setTitleVisible(!style()->separateTitle() && style()->hasTitle());
    if (d->title) {
        d->title->setObjectName(style()->titleObjectName());
    }
}

void MComboBoxView::updateData(const QList<const char *>& modifications)
{
    Q_D(MComboBoxView);

    MWidgetView::updateData(modifications);

    foreach(const char* member, modifications) {
        if (member == MComboBoxModel::IconID) {
            d->buttonWidget()->setIconID(model()->iconID());
            d->buttonWidget()->setIconVisible(model()->iconVisible() && !model()->iconID().isEmpty());
        } else if (member == MComboBoxModel::IconVisible) {
            d->buttonWidget()->setIconVisible(model()->iconVisible() && !model()->iconID().isEmpty());
        } else if (member == MComboBoxModel::ProgressIndicatorVisible) {
            d->buttonWidget()->setProgressIndicatorVisible(model()->progressIndicatorVisible());
        } else if (member == MComboBoxModel::Title) {
            d->updateTitle(model()->title());
        } else if (member == MComboBoxModel::ItemModel) {
            if (d->popuplist)
                d->popuplist->setItemModel(model()->itemModel());
        } else if (member == MComboBoxModel::SelectionModel) {
            if (d->popuplist)
                d->popuplist->setSelectionModel(model()->selectionModel());
        } else if (member == MWidgetModel::LayoutPosition) {
            d->buttonWidget()->setLayoutPosition(model()->layoutPosition());
        }
    }
}

void MComboBoxView::setupModel()
{
    Q_D(MComboBoxView);
    MWidgetView::setupModel();

    d->updateTitle(model()->title());
    d->buttonWidget()->setIconID(model()->iconID());
    d->buttonWidget()->setIconVisible(model()->iconVisible() && !model()->iconID().isEmpty());
    d->buttonWidget()->setProgressIndicatorVisible(model()->progressIndicatorVisible());
    d->buttonWidget()->setLayoutPosition(model()->layoutPosition());
    d->buttonWidget()->setViewType(model()->viewType());
    d->updateSubtitle(d->controller->currentIndex());
}

M_REGISTER_VIEW_NEW(MComboBoxView, MComboBox)

#include "moc_mcomboboxview.cpp"
