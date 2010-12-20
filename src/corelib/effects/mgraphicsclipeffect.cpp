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
 
#include "mgraphicsclipeffect.h"
#include "mgraphicsclipeffect_p.h"

#include <QPainter>

MGraphicsClipEffectPrivate::MGraphicsClipEffectPrivate()
    : q_ptr(0),
      drawRectangle(0, 0, 0, 0),
      margin(0)
{
}

MGraphicsClipEffectPrivate::~MGraphicsClipEffectPrivate()
{
}

MGraphicsClipEffect::MGraphicsClipEffect(QObject *parent)
    : QGraphicsEffect(parent),
    d_ptr(new MGraphicsClipEffectPrivate)
{
    Q_D(MGraphicsClipEffect);
    d->q_ptr = this;
}

MGraphicsClipEffect::~MGraphicsClipEffect()
{
    delete d_ptr;
}

void MGraphicsClipEffect::setDrawRect(const QRect &rect)
{
    Q_D(MGraphicsClipEffect);
    d->drawRectangle = rect;
    update();
}

QRect MGraphicsClipEffect::drawRect() const
{
    Q_D(const MGraphicsClipEffect);
    return d->drawRectangle;
}

void MGraphicsClipEffect::setClipMargin(int margin)
{
    Q_D(MGraphicsClipEffect);
    d->margin = margin;
    update();
}

int MGraphicsClipEffect::clipMargin() const
{
    Q_D(const MGraphicsClipEffect);
    return d->margin;
}

void MGraphicsClipEffect::draw(QPainter *painter)
{
    Q_D(MGraphicsClipEffect);

    QPoint offset;
    QPixmap pixmap;

    pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);

    QPoint topPosition = QPoint(offset.x(), offset.y() + d->drawRectangle.y());
    QPoint bottomPosition = QPoint(offset.x(), offset.y() + d->drawRectangle.y() + d->drawRectangle.height());
    QPoint topPartPosition = QPoint(topPosition.x(), topPosition.y() - d->margin);

    if (d->drawRectangle.height() > pixmap.rect().height() - d->margin * 2) {
        topPartPosition.setY(offset.y());
        bottomPosition.setY(offset.y() + pixmap.rect().height() - d->margin);
    }

    painter->drawPixmap(topPosition, pixmap,
                        QRect(d->drawRectangle.x(),  d->drawRectangle.y(), d->drawRectangle.size().width(),  d->drawRectangle.height()));
    painter->drawPixmap(topPartPosition, pixmap,
                        QRect(0, 0, d->drawRectangle.size().width(), d->margin));
    painter->drawPixmap(QPoint(bottomPosition.x(), bottomPosition.y()), pixmap,
                        QRect(0, pixmap.rect().height() - d->margin, d->drawRectangle.size().width(), d->margin));
}
