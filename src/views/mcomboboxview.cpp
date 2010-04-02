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

#include "mtheme.h"
#include "mimagewidget.h"
#include "mlabel.h"
#include "mscalableimage.h"
#include "mviewcreator.h"
#include "mpopuplist.h"
#include "mapplicationwindow.h"
#include "mscenemanager.h"
#include "mlocale.h"
#include "mcontentitem.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

MComboBoxViewPrivate::MComboBoxViewPrivate()
    : q_ptr(0), controller(0), contentItem(0), popuplist(0), pixmap(0), progressIndicator(0)
{
}

MComboBoxViewPrivate::~MComboBoxViewPrivate()
{
    delete contentItem;
    delete popuplist;
    delete pixmap;
    delete progressIndicator;
}

void MComboBoxViewPrivate::init()
{
    Q_Q(MComboBoxView);

    layout = new QGraphicsLinearLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    contentItem = new MContentItem(MContentItem::TwoIconsTwoWidgets);
    layout->addItem(contentItem);
    updateSubtitle(controller->currentIndex());

    QObject::connect(contentItem, SIGNAL(clicked()), controller, SLOT(click()));
    QObject::connect(controller, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_itemModelCurrentIndexChanged(int)));
    QObject::connect(controller, SIGNAL(clicked()), q, SLOT(_q_showPopup()));
}

void MComboBoxViewPrivate::initLayout()
{
    Q_Q(MComboBoxView);

    MContentItem::ContentItemStyle newStyle;

    if (controller->isIconVisible() && !controller->iconID().isEmpty())
        newStyle = MContentItem::TwoIconsTwoWidgets;
    else if (controller->isProgressIndicatorVisible())
        newStyle = MContentItem::TwoIconsTwoWidgets;
    else
        newStyle = MContentItem::TwoTextLabels;

    if (contentItem->itemStyle() != newStyle) {
        delete contentItem;

        contentItem = new MContentItem(newStyle);
        contentItem->setObjectName(q->style()->contentItemObjectName());
        contentItem->setItemMode((MContentItem::ContentItemMode)q->style()->itemMode());
        layout->addItem(contentItem);
        updateSubtitle(controller->currentIndex());
        contentItem->setTitle(controller->title());

        QObject::connect(contentItem, SIGNAL(clicked()), controller, SLOT(click()));
    }

    if (controller->isIconVisible() && !controller->iconID().isEmpty()) {
        // TODO: Use MTheme::pixmap() when MContentItem starts to support
        //       pixmaps that are loaded asynchronously
        if (!pixmap)
            pixmap = MTheme::pixmapCopy(controller->iconID());
        contentItem->setPixmap(*pixmap);
    }

    if (controller->isProgressIndicatorVisible()) {
        if (!progressIndicator) {
            progressIndicator = new MProgressIndicator;
            progressIndicator->setViewType(MProgressIndicator::spinnerType);
            progressIndicator->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
            progressIndicator->setUnknownDuration(true);
        }
        else {
            progressIndicator->setVisible(true);
        }
        contentItem->setAdditionalItem(progressIndicator);
    } else {
        if (progressIndicator)
            progressIndicator->setVisible(false);
        contentItem->setAdditionalItem(NULL);
    }
}

void MComboBoxViewPrivate::updateSubtitle(int currentIndex) {
    if (currentIndex != -1)  {
        contentItem->setSubtitle(controller->itemText(currentIndex));
    } else {
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.doc
        //: default value for Popup List button sublabel when nothing has been selected yet
        //% "Tap to Select"
        contentItem->setSubtitle(qtTrId("xx_ComboBoxSubtitle"));
    }
}

void MComboBoxViewPrivate::_q_itemModelCurrentIndexChanged(int currentIndex)
{
    updateSubtitle(currentIndex);
}

void MComboBoxViewPrivate::_q_showPopup()
{
    if (controller->count() == 0) return;

    if (!popuplist) {
        popuplist = new MPopupList();
        popuplist->setItemModel(controller->itemModel());
        popuplist->setSelectionModel(controller->selectionModel());
        popuplist->setTitle(controller->title());
        QObject::connect(popuplist, SIGNAL(clicked(QModelIndex)), controller, SLOT(_q_itemClicked(QModelIndex)));
    }

    if (controller->sceneManager()) {
        controller->sceneManager()->appearSceneWindow(popuplist);
    }
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

    d->contentItem->setObjectName(style()->contentItemObjectName());
    d->contentItem->setItemMode((MContentItem::ContentItemMode)style()->itemMode());
}

void MComboBoxView::updateData(const QList<const char *>& modifications)
{
    Q_D(MComboBoxView);

    MWidgetView::updateData(modifications);

    foreach(const char* member, modifications) {
        if (member == MComboBoxModel::IconID) {
            delete d->pixmap;
            d->pixmap = NULL;
            d->initLayout();
        } else if (member == MComboBoxModel::IconVisible ||
                   member == MComboBoxModel::ProgressIndicatorVisible)
        {
            d->initLayout();
        } else if (member == MComboBoxModel::Title) {
            d->contentItem->setTitle(model()->title());
            if (d->popuplist)
                d->popuplist->setTitle(model()->title());
        } else if (member == MComboBoxModel::ItemModel) {
            if (d->popuplist)
                d->popuplist->setItemModel(model()->itemModel());
        } else if (member == MComboBoxModel::SelectionModel) {
            if (d->popuplist)
                d->popuplist->setSelectionModel(model()->selectionModel());
        }
    }
}

void MComboBoxView::setupModel()
{
    Q_D(MComboBoxView);
    MWidgetView::setupModel();

    d->initLayout();

    d->contentItem->setTitle(model()->title());
}

M_REGISTER_VIEW_NEW(MComboBoxView, MComboBox)

#include "moc_mcomboboxview.cpp"
