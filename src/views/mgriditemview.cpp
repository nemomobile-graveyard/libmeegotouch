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

#include "mgriditemview.h"
#include "mgriditemview_p.h"
#include "mgriditem.h"

#include "mscalableimage.h"
#include "mtheme.h"
#include "mimagewidget.h"
#include "mlabel.h"
#include "mviewcreator.h"

#include <MButton>

#include <QGraphicsGridLayout>
#include <QDebug>

MGridItemViewPrivate::MGridItemViewPrivate()
    : q_ptr(0), controller(0), layout(0), title(0), subtitle(0), image(0), layoutDirty(false)
{
}

MGridItemViewPrivate::~MGridItemViewPrivate()
{
}

// Use lazy initialize to reduce useless widget and get better performance
void MGridItemViewPrivate::init()
{
    layout = new QGraphicsGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    iconAlign = Qt::AlignLeft;
}

void MGridItemViewPrivate::initLayout() const
{
    int count = layout->count();
    for (int i = count - 1; i >= 0; --i)
        layout->removeAt(i);

    MGridItemModel *model = (MGridItemModel *)controller->model();
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

void MGridItemViewPrivate::initTitle() const
{
    if (title == 0) {
        title = new MLabel();
        title->setObjectName("MGridItemTitle");
    }
}

void MGridItemViewPrivate::initSubtitle() const
{
    if (subtitle == 0) {
        subtitle = new MLabel();
        subtitle->setObjectName("MGridItemSubtitle");
    }
}

void MGridItemViewPrivate::initImage() const
{
    if (image == 0) {
        image = new MImageWidget();
        image->setObjectName("MGridItemImage");
    }
}

void MGridItemViewPrivate::setTitle(const QString &text)
{
    if (text.isEmpty()) return;
    initTitle();
    title->setText(text);
}

void MGridItemViewPrivate::setSubtitle(const QString &text)
{
    if (text.isEmpty()) return;
    initSubtitle();
    subtitle->setText(text);
}

void MGridItemViewPrivate::setImage(const QString &name)
{
    if (name.isEmpty() && controller->pixmap().isNull()) return;

    Q_Q(MGridItemView);
    initImage();

    if (name.isEmpty())
        image->setPixmap(controller->pixmap());
    else
        image->setImage(name, q->style()->iconSize());
}

void MGridItemViewPrivate::_q_updateImage()
{
    initImage();
    image->setPixmap(controller->pixmap());
}

MGridItemView::MGridItemView(MGridItem *controller) :
    MWidgetView(controller),
    d_ptr(new MGridItemViewPrivate)
{
    Q_D(MGridItemView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();

    connect(controller, SIGNAL(pixmapChanged()), this, SLOT(_q_updateImage()));
}

MGridItemView::MGridItemView(MGridItemViewPrivate &dd, MGridItem *controller) :
    MWidgetView(controller),
    d_ptr(&dd)
{
    Q_D(MGridItemView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();

    connect(controller, SIGNAL(pixmapChanged()), this, SLOT(_q_updateImage()));
}

MGridItemView::~MGridItemView()
{
    delete d_ptr;
}

void MGridItemView::applyStyle()
{
    Q_D(MGridItemView);

    MWidgetView::applyStyle();

    // change the icon Alignment
    if (d->iconAlign != style()->iconAlign()) {
        d->iconAlign = style()->iconAlign();
        d->layoutDirty = true;
    }
}

void MGridItemView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MGridItemView);
    if (d->layoutDirty)
        d->initLayout();

    MWidgetView::drawContents(painter, option);
}

void MGridItemView::updateData(const QList<const char *>& modifications)
{
    Q_D(MGridItemView);

    MWidgetView::updateData(modifications);

    const char *member;
    const int count = modifications.count();
    for (int i = 0; i < count; ++i) {
        member = modifications[i];

        if (member == MGridItemModel::Image) {
            d->setImage(model()->image());
        } else if (member == MGridItemModel::Title) {
            d->setTitle(model()->title());
        } else if (member == MGridItemModel::Subtitle) {
            d->setSubtitle(model()->subtitle());
        } else if (member == MGridItemModel::ImageVisible || member == MGridItemModel::TitleVisible ||
                   member == MGridItemModel::SubtitleVisible) {
            d->layoutDirty = true;
        }
    }
}

void MGridItemView::setupModel()
{
    Q_D(MGridItemView);
    MWidgetView::setupModel();

    d->setTitle(model()->title());
    d->setSubtitle(model()->subtitle());
    d->setImage(model()->image());
    d->layoutDirty = true;
}

M_REGISTER_VIEW_NEW(MGridItemView, MGridItem)

#include "moc_mgriditemview.cpp"

