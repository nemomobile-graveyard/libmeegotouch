/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duicomboboxview.h"
#include "duicomboboxview_p.h"
#include "duicombobox.h"

#include "duitheme.h"
#include "duiimagewidget.h"
#include "duilabel.h"
#include "duiscalableimage.h"
#include "duiviewcreator.h"
#include "duipopuplist.h"
#include "duiapplicationwindow.h"
#include "duiscenemanager.h"
#include "duilocale.h"
#include "duicontentitem.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

DuiComboBoxViewPrivate::DuiComboBoxViewPrivate()
    : controller(0), contentItem(0), popuplist(0)
{
}

DuiComboBoxViewPrivate::~DuiComboBoxViewPrivate()
{
    delete popuplist;
}

void DuiComboBoxViewPrivate::init()
{
    Q_Q(DuiComboBoxView);

    layout = new QGraphicsLinearLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    contentItem = new DuiContentItem(DuiContentItem::TwoTextLabels);
    layout->addItem(contentItem);
    updateSubtitle(controller->currentIndex());

    QObject::connect(contentItem, SIGNAL(clicked()), controller, SLOT(click()));
    QObject::connect(controller, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_itemModelCurrentIndexChanged(int)));
    QObject::connect(controller, SIGNAL(clicked()), q, SLOT(_q_showPopup()));
}

void DuiComboBoxViewPrivate::initLayout()
{
    Q_Q(DuiComboBoxView);

    DuiContentItem::ContentItemStyle newStyle;;

    if (controller->isIconVisible() && !controller->iconID().isEmpty())
        newStyle = DuiContentItem::IconAndTwoTextLabels;
    else
        newStyle = DuiContentItem::TwoTextLabels;

    if (contentItem->itemStyle() != newStyle) {
        delete contentItem;

        contentItem = new DuiContentItem(newStyle);
        contentItem->setObjectName(q->style()->contentItemObjectName());
        contentItem->setItemMode((DuiContentItem::ContentItemMode)q->style()->itemMode());
        layout->addItem(contentItem);
        updateSubtitle(controller->currentIndex());
        contentItem->setTitle(controller->title());
        if (newStyle == DuiContentItem::IconAndTwoTextLabels)
            contentItem->setPixmap(*DuiTheme::pixmap(controller->iconID()));

        QObject::connect(contentItem, SIGNAL(clicked()), controller, SLOT(click()));
    }
}

void DuiComboBoxViewPrivate::updateSubtitle(int currentIndex) {
    if (currentIndex != -1)  {
        contentItem->setSubtitle(controller->itemText(currentIndex));
    } else {
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.doc
        //: default value for Popup List button sublabel when nothing has been selected yet
        //% "Tap to Select"
        contentItem->setSubtitle(qtTrId("xx_ComboBoxSubtitle"));
    }
}

void DuiComboBoxViewPrivate::_q_itemModelCurrentIndexChanged(int currentIndex)
{
    updateSubtitle(currentIndex);
}

void DuiComboBoxViewPrivate::_q_showPopup()
{
    if (controller->count() == 0) return;

    if (!popuplist) {
        popuplist = new DuiPopupList();
        popuplist->setItemModel(controller->itemModel());
        popuplist->setSelectionModel(controller->selectionModel());
        popuplist->setTitle(controller->title());
        QObject::connect(popuplist, SIGNAL(clicked(QModelIndex)), controller, SLOT(_q_itemClicked(QModelIndex)));
    }

    if (controller->sceneManager()) {
        controller->sceneManager()->appearSceneWindow(popuplist);
    }
}

DuiComboBoxView::DuiComboBoxView(DuiComboBox *controller) :
    DuiWidgetView(* new DuiComboBoxViewPrivate, controller)
{
    Q_D(DuiComboBoxView);
    d->controller = controller;
    d->init();

}

DuiComboBoxView::DuiComboBoxView(DuiComboBoxViewPrivate &dd, DuiComboBox *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiComboBoxView);
    d->controller = controller;
    d->init();
}

DuiComboBoxView::~DuiComboBoxView()
{
}

void DuiComboBoxView::applyStyle()
{
    Q_D(DuiComboBoxView);

    DuiWidgetView::applyStyle();

    d->contentItem->setObjectName(style()->contentItemObjectName());
    d->contentItem->setItemMode((DuiContentItem::ContentItemMode)style()->itemMode());
}

void DuiComboBoxView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiComboBoxView);

    DuiWidgetView::updateData(modifications);

    foreach(const char* member, modifications) {
        if (member == DuiComboBoxModel::IconID || member == DuiComboBoxModel::IconVisible) {
            d->initLayout();
        } else if (member == DuiComboBoxModel::Title) {
            d->contentItem->setTitle(model()->title());
            if (d->popuplist)
                d->popuplist->setTitle(model()->title());
        } else if (member == DuiComboBoxModel::ItemModel) {
            if (d->popuplist)
                d->popuplist->setItemModel(model()->itemModel());
        } else if (member == DuiComboBoxModel::SelectionModel) {
            if (d->popuplist)
                d->popuplist->setSelectionModel(model()->selectionModel());
        }
    }
}

void DuiComboBoxView::setupModel()
{
    Q_D(DuiComboBoxView);
    DuiWidgetView::setupModel();

    d->initLayout();

    d->contentItem->setTitle(model()->title());
}

DUI_REGISTER_VIEW_NEW(DuiComboBoxView, DuiComboBox)

#include "moc_duicomboboxview.cpp"
