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

#include "duiinfobannerinformationview.h"
#include "duiinfobannerinformationview_p.h"

#include "duiinfobanner.h"
#include "duiviewcreator.h"
#include "duiimagewidget.h"
#include "duilabel.h"
#include "duibutton.h"

#include <QGraphicsGridLayout>
#include <QGraphicsSceneMouseEvent>

DuiInfoBannerInformationViewPrivate::DuiInfoBannerInformationViewPrivate(DuiInfoBanner *banner) :
    q_ptr(0), banner(banner), label(0), image(0), button(0), layout(0),
    controller(banner)
{
}

DuiInfoBannerInformationViewPrivate::~DuiInfoBannerInformationViewPrivate()
{
}

void DuiInfoBannerInformationViewPrivate::init()
{
    label = new DuiLabel();
    label->setAlignment(Qt::AlignCenter);
    label->setTextElide(true);
    label->setObjectName("SystemInfoBannerBody");

    image = new DuiImageWidget();
    image->setObjectName("SystemInfoBannerImage");

    layout = new QGraphicsGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addItem(image, 0, 0);
    layout->addItem(label, 0, 1);

    button = new DuiButton(controller);
    button->setObjectName("SystemInfoBannerButton");

    controller->setLayout(layout);
}

void DuiInfoBannerInformationViewPrivate::setButtonText(const QString &text)
{
    if (!text.isEmpty()) {
        if (layout->count() == 2)
            layout->addItem(button, 1, 0, 1, 2);
        button->setText(text);
        button->setVisible(true);
    } else {
        if (layout->count() == 3)
            layout->removeAt(2);
        button->setVisible(false);
    }

}

DuiInfoBannerInformationView::DuiInfoBannerInformationView(DuiInfoBanner *controller) :
    DuiSceneWindowView(controller),
    d_ptr(new DuiInfoBannerInformationViewPrivate(controller))
{
    Q_D(DuiInfoBannerInformationView);
    d->q_ptr = this;
    d->init();
}

DuiInfoBannerInformationView::DuiInfoBannerInformationView(DuiInfoBannerInformationViewPrivate &dd, DuiInfoBanner *controller) :
    DuiSceneWindowView(controller),
    d_ptr(&dd)
{
    Q_D(DuiInfoBannerInformationView);
    d->q_ptr = this;
    d->init();
}

DuiInfoBannerInformationView::~DuiInfoBannerInformationView()
{
    delete d_ptr;
}

QSizeF DuiInfoBannerInformationView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiInfoBannerInformationView);

    QSizeF base = DuiSceneWindowView::sizeHint(which, constraint);

    QSizeF buttonSize(0, 0);
    if (!model()->buttonText().isEmpty())
        buttonSize = d->button->sizeHint(which, constraint);

    base.setHeight(base.height() + buttonSize.height());

    return base;
}

void DuiInfoBannerInformationView::updateData(const QList<const char *>& modifications)
{
    DuiSceneWindowView::updateData(modifications);

    Q_D(DuiInfoBannerInformationView);
    const char *member;

    const int count = modifications.count();
    for (int i = 0; i < count; ++i) {
        member = modifications[i];
        if (member == DuiInfoBannerModel::BodyText) {
            d->label->setText(model()->bodyText());
        } else if (member == DuiInfoBannerModel::ImageID) {
            d->image->setImage(model()->imageID(), style()->imageSize());
        } else if (member == DuiInfoBannerModel::Pixmap) {
            d->image->setPixmap(model()->pixmap());
        } else if (member == DuiInfoBannerModel::ButtonText) {
            d->setButtonText(model()->buttonText());
        }
    }
}

void DuiInfoBannerInformationView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void DuiInfoBannerInformationView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiInfoBannerInformationView);

    QPointF touch = event->scenePos();
    QRectF rect  = d->controller->sceneBoundingRect();
    if (rect.contains(touch)) {
        d->banner->click();
    }
}

void DuiInfoBannerInformationView::setupModel()
{
    DuiSceneWindowView::setupModel();

    Q_D(DuiInfoBannerInformationView);

    QObject::connect(d->button, SIGNAL(clicked()), d->banner, SIGNAL(buttonClicked()));
    d->label->setText(model()->bodyText());
    d->setButtonText(model()->buttonText());

    if (!model()->imageID().isEmpty()) {
        d->image->setImage(model()->imageID(), style()->imageSize());
    } else {
        d->image->setPixmap(model()->pixmap());
    }
}

void DuiInfoBannerInformationView::applyStyle()
{
    DuiSceneWindowView::applyStyle();

    Q_D(DuiInfoBannerInformationView);

    if (!model()->imageID().isEmpty()) {
        d->image->setImage(model()->imageID(), style()->imageSize());
    } else {
        d->image->setPixmap(model()->pixmap());
    }
}

DUI_REGISTER_VIEW_NEW(DuiInfoBannerInformationView, DuiInfoBanner)
