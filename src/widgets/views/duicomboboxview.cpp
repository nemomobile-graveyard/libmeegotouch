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

#include <QGraphicsGridLayout>
#include <QGraphicsSceneMouseEvent>

DuiComboBoxViewPrivate::DuiComboBoxViewPrivate()
    : controller(0), layout(0), title(0), subtitle(0), icon(0), popuplist(0)
{
}

DuiComboBoxViewPrivate::~DuiComboBoxViewPrivate()
{
    if (popuplist != 0)
        delete popuplist;
}

void DuiComboBoxViewPrivate::init()
{
    Q_Q(DuiComboBoxView);

    layout = new QGraphicsGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    icon = new DuiImageWidget();
    icon->setObjectName("DuiComboBoxIcon");

    title = new DuiLabel();
    title->setObjectName("DuiComboBoxTitle");

    subtitle = new DuiLabel();
    subtitle->setObjectName("DuiComboBoxSubtitle");
    _q_itemModelCurrentIndexChanged(-1);

    QObject::connect(controller, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_itemModelCurrentIndexChanged(int)));
    QObject::connect(controller, SIGNAL(clicked()), q, SLOT(_q_showPopup()));
}

void DuiComboBoxViewPrivate::initLayout()
{
    int count = layout->count();
    for (int i = count - 1; i >= 0; --i)
        layout->removeAt(i);

    if (controller->isIconVisible() && !icon->image().isEmpty()) {
        icon->show();
        layout->addItem(icon, 0, 0, 2, 1);
        layout->addItem(title, 0, 1);
        layout->addItem(subtitle, 1, 1);
    } else {
        icon->hide();
        layout->addItem(title, 0, 0);
        layout->addItem(subtitle, 1, 0);
    }
}

void DuiComboBoxViewPrivate::_q_itemModelCurrentIndexChanged(int currentIndex)
{
    if (currentIndex != -1)  {
        subtitle->setText(controller->itemText(currentIndex));
    } else {
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.doc
        //: default value for Popup List button sublabel when nothing has been selected yet
        //% "Tap to Select"
        subtitle->setText(qtTrId("xx_ComboBoxSubtitle"));
    }
}

void DuiComboBoxViewPrivate::_q_showPopup()
{
    if (controller->count() == 0) return;

    if (controller->sceneManager()) {
        if (!popuplist) {
            popuplist = new DuiPopupList();
        }

        popuplist->setItemModel(controller->itemModel());
        popuplist->setSelectionModel(controller->selectionModel());
        popuplist->setTitle(controller->title());
        controller->sceneManager()->showWindow(popuplist);
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

void DuiComboBoxView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(DuiComboBoxView);

    if (d->controller->isDown())
        return;

    d->controller->setDown(true);
}

void DuiComboBoxView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiComboBoxView);

    if (!d->controller->isDown())
        return;

    d->controller->setDown(false);

    QPointF touch = event->scenePos();
    QRectF rect  = d->controller->sceneBoundingRect();
    if (rect.contains(touch)) {
        d->controller->click();
    }
}

void DuiComboBoxView::cancelEvent(DuiCancelEvent *event)
{
    Q_D(DuiComboBoxView);
    Q_UNUSED(event);

    if (!d->controller->isDown())
        return;

    d->controller->setDown(false);

    update();
}

void DuiComboBoxView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiComboBoxView);

    DuiWidgetView::updateData(modifications);

    const char *member;
    const int count = modifications.count();
    for (int i = 0; i < count; ++i) {
        member = modifications[i];

        if (member == DuiComboBoxModel::IconID) {
            d->icon->setImage(model()->iconID());
            d->initLayout();
        } else if (member == DuiComboBoxModel::Title) {
            QString text = model()->title();
            d->title->setText(text);
            if (d->popuplist)
                d->popuplist->setTitle(text);
        } else if (member == DuiComboBoxModel::IconVisible) {
            d->initLayout();
        } else if (member == DuiComboBoxModel::Down) {
            if (model()->down())
                style().setModePressed();
            else
                style().setModeDefault();
            update();
        }
    }
}

void DuiComboBoxView::setupModel()
{
    Q_D(DuiComboBoxView);
    DuiWidgetView::setupModel();

    d->initLayout();

    d->icon->setImage(model()->iconID());
    d->title->setText(model()->title());
    if (!d->controller->currentText().isEmpty())
        d->subtitle->setText(d->controller->currentText());
}

DUI_REGISTER_VIEW_NEW(DuiComboBoxView, DuiComboBox)

#include "moc_duicomboboxview.cpp"
