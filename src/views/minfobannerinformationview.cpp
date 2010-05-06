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

#include "minfobannerinformationview.h"
#include "minfobannerinformationview_p.h"

#include "minfobanner.h"
#include "mviewcreator.h"
#include "mimagewidget.h"
#include "mlabel.h"
#include "mbutton.h"

#include <QGraphicsGridLayout>
#include <QGraphicsSceneMouseEvent>

MInfoBannerInformationViewPrivate::MInfoBannerInformationViewPrivate(MInfoBanner *banner) :
    q_ptr(0), banner(banner), label(0), image(0), button(0), layout(0),
    controller(banner)
{
}

MInfoBannerInformationViewPrivate::~MInfoBannerInformationViewPrivate()
{
}

void MInfoBannerInformationViewPrivate::init()
{
    label = new MLabel();
    label->setAlignment(Qt::AlignCenter);
    label->setTextElide(true);
    label->setObjectName("SystemInfoBannerBody");

    image = new MImageWidget();
    image->setObjectName("SystemInfoBannerImage");

    layout = new QGraphicsGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addItem(image, 0, 0);
    layout->addItem(label, 0, 1);

    button = new MButton(controller);
    button->setObjectName("SystemInfoBannerButton");

    controller->setLayout(layout);

    QObject::connect(banner, SIGNAL(clicked()), controller, SLOT(dismiss()));
}

void MInfoBannerInformationViewPrivate::setButtonText(const QString &text)
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

MInfoBannerInformationView::MInfoBannerInformationView(MInfoBanner *controller) :
    MSceneWindowView(controller),
    d_ptr(new MInfoBannerInformationViewPrivate(controller))
{
    Q_D(MInfoBannerInformationView);
    d->q_ptr = this;
    d->init();
}

MInfoBannerInformationView::MInfoBannerInformationView(MInfoBannerInformationViewPrivate &dd, MInfoBanner *controller) :
    MSceneWindowView(controller),
    d_ptr(&dd)
{
    Q_D(MInfoBannerInformationView);
    d->q_ptr = this;
    d->init();
}

MInfoBannerInformationView::~MInfoBannerInformationView()
{
    delete d_ptr;
}

QSizeF MInfoBannerInformationView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MInfoBannerInformationView);

    QSizeF base = MSceneWindowView::sizeHint(which, constraint);

    QSizeF buttonSize(0, 0);
    if (!model()->buttonText().isEmpty())
        buttonSize = d->button->sizeHint(which, constraint);

    base.setHeight(base.height() + buttonSize.height());

    return base;
}

void MInfoBannerInformationView::updateData(const QList<const char *>& modifications)
{
    MSceneWindowView::updateData(modifications);

    Q_D(MInfoBannerInformationView);
    const char *member;

    const int count = modifications.count();
    for (int i = 0; i < count; ++i) {
        member = modifications[i];
        if (member == MInfoBannerModel::BodyText) {
            d->label->setText(model()->bodyText());
        } else if (member == MInfoBannerModel::ImageID) {
            d->image->setImage(model()->imageID(), style()->imageSize());
        } else if (member == MInfoBannerModel::Pixmap) {
            d->image->setPixmap(model()->pixmap());
        } else if (member == MInfoBannerModel::ButtonText) {
            d->setButtonText(model()->buttonText());
        }
    }
}

void MInfoBannerInformationView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MInfoBannerInformationView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MInfoBannerInformationView);

    QPointF touch = event->scenePos();
    QRectF rect  = d->controller->sceneBoundingRect();
    if (rect.contains(touch)) {
        d->banner->click();
    }
}

void MInfoBannerInformationView::setupModel()
{
    MSceneWindowView::setupModel();

    Q_D(MInfoBannerInformationView);

    QObject::connect(d->button, SIGNAL(clicked()), d->banner, SIGNAL(buttonClicked()));
    QObject::connect(d->button, SIGNAL(clicked()), d->banner, SLOT(dismiss()));

    d->label->setText(model()->bodyText());
    d->setButtonText(model()->buttonText());

    if (!model()->imageID().isEmpty()) {
        d->image->setImage(model()->imageID(), style()->imageSize());
    } else {
        d->image->setPixmap(model()->pixmap());
    }
}

void MInfoBannerInformationView::applyStyle()
{
    MSceneWindowView::applyStyle();

    Q_D(MInfoBannerInformationView);

    if (!model()->imageID().isEmpty()) {
        d->image->setImage(model()->imageID(), style()->imageSize());
    } else {
        d->image->setPixmap(model()->pixmap());
    }
}

M_REGISTER_VIEW_NEW(MInfoBannerInformationView, MInfoBanner)
