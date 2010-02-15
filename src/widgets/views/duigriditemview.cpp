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

#include "duigriditemview.h"
#include "duigriditemview_p.h"
#include "duigriditem.h"

#include "duiscalableimage.h"
#include "duitheme.h"
#include "duiimagewidget.h"
#include "duilabel.h"
#include "duiviewcreator.h"

#include <QGraphicsGridLayout>
#include <QDebug>

DuiGridItemViewPrivate::DuiGridItemViewPrivate()
    : controller(0), layout(0), title(0), subtitle(0), image(0), layoutDirty(false)
{
}

DuiGridItemViewPrivate::~DuiGridItemViewPrivate()
{
}

// Use lazy initialize to reduce useless widget and get better performance
void DuiGridItemViewPrivate::init()
{
    layout = new QGraphicsGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    iconAlign = Qt::AlignLeft;
}

void DuiGridItemViewPrivate::initLayout() const
{
    int count = layout->count();
    for (int i = count - 1; i >= 0; --i)
        layout->removeAt(i);

    DuiGridItemModel *model = (DuiGridItemModel *)controller->model();
    if (!model->imageVisible()) {
        // Label
        initTitle();
        layout->addItem(title, 0, 0);
        layout->setAlignment(title,  Qt::AlignVCenter | Qt::AlignHCenter);
    } else {
        if (!model->titleVisible() && !model->subtitleVisible()) {
            // Image
            initImage();
            layout->addItem(image, 0, 0);
            layout->setAlignment(image,  Qt::AlignVCenter | Qt::AlignHCenter);
        } else if (!model->subtitleVisible()) {
            // ImageLabel
            initTitle();
            initImage();

            if (iconAlign == Qt::AlignLeft) {
                layout->addItem(image, 0, 0);
                layout->addItem(title, 0, 1);
            } else {
                layout->addItem(title, 0, 0);
                layout->addItem(image, 0, 1);
            }
        } else {
            // Grid Item
            initTitle();
            initSubtitle();
            initImage();

            if (iconAlign == Qt::AlignLeft) {
                layout->addItem(image, 0, 0, 2, 1);
                layout->addItem(title, 0, 1);
                layout->addItem(subtitle, 1, 1);
            } else {
                layout->addItem(image, 0, 1, 2, 1);
                layout->addItem(title, 0, 0);
                layout->addItem(subtitle, 1, 0);
            }
        }
    }

    layoutDirty = false;
}

void DuiGridItemViewPrivate::initTitle() const
{
    if (title == 0) {
        title = new DuiLabel();
        title->setObjectName("DuiGridItemTitle");
    }
}

void DuiGridItemViewPrivate::initSubtitle() const
{
    if (subtitle == 0) {
        subtitle = new DuiLabel();
        subtitle->setObjectName("DuiGridItemSubtitle");
    }
}

void DuiGridItemViewPrivate::initImage() const
{
    if (image == 0) {
        image = new DuiImageWidget();
        image->setObjectName("DuiGridItemImage");
    }
}

void DuiGridItemViewPrivate::setTitle(const QString &text)
{
    if (text.isEmpty()) return;
    initTitle();
    title->setText(text);
}

void DuiGridItemViewPrivate::setSubtitle(const QString &text)
{
    if (text.isEmpty()) return;
    initSubtitle();
    subtitle->setText(text);
}

void DuiGridItemViewPrivate::setImage(const QString &name)
{
    if (name.isEmpty() && controller->pixmap().isNull()) return;

    Q_Q(DuiGridItemView);
    initImage();

    if (name.isEmpty())
        image->setPixmap(controller->pixmap());
    else
        image->setImage(name, q->style()->iconSize());
}

void DuiGridItemViewPrivate::_q_updateImage()
{
    initImage();
    image->setPixmap(controller->pixmap());
}

DuiGridItemView::DuiGridItemView(DuiGridItem *controller) :
    DuiWidgetView(* new DuiGridItemViewPrivate, controller)
{
    Q_D(DuiGridItemView);
    d->controller = controller;
    d->init();

    connect(controller, SIGNAL(pixmapChanged()), this, SLOT(_q_updateImage()));
}

DuiGridItemView::DuiGridItemView(DuiGridItemViewPrivate &dd, DuiGridItem *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiGridItemView);
    d->controller = controller;
    d->init();

    connect(controller, SIGNAL(pixmapChanged()), this, SLOT(_q_updateImage()));
}

DuiGridItemView::~DuiGridItemView()
{
}

void DuiGridItemView::applyStyle()
{
    Q_D(DuiGridItemView);

    DuiWidgetView::applyStyle();

    // change the icon Alignment
    if (d->iconAlign != style()->iconAlign()) {
        d->iconAlign = style()->iconAlign();
        d->layoutDirty = true;
    }
}

void DuiGridItemView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const DuiGridItemView);
    if (d->layoutDirty)
        d->initLayout();

    DuiWidgetView::drawContents(painter, option);
}

void DuiGridItemView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiGridItemView);

    DuiWidgetView::updateData(modifications);

    const char *member;
    const int count = modifications.count();
    for (int i = 0; i < count; ++i) {
        member = modifications[i];

        if (member == DuiGridItemModel::Image) {
            d->setImage(model()->image());
        } else if (member == DuiGridItemModel::Title) {
            d->setTitle(model()->title());
        } else if (member == DuiGridItemModel::Subtitle) {
            d->setSubtitle(model()->subtitle());
        } else if (member == DuiGridItemModel::ImageVisible || member == DuiGridItemModel::TitleVisible ||
                   member == DuiGridItemModel::SubtitleVisible) {
            d->layoutDirty = true;
        }
    }
}

void DuiGridItemView::setupModel()
{
    Q_D(DuiGridItemView);
    DuiWidgetView::setupModel();

    d->setTitle(model()->title());
    d->setSubtitle(model()->subtitle());
    d->setImage(model()->image());
    d->layoutDirty = true;
}

DUI_REGISTER_VIEW_NEW(DuiGridItemView, DuiGridItem)

#include "moc_duigriditemview.cpp"

