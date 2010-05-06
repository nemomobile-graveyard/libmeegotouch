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

#include "mextendingbackgroundview.h"
#include "mextendingbackgroundview_p.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <MWidgetController>
//#include <MSceneManager>
#include <MApplication>
#include <MApplicationWindow>
#include <MScalableImage>
#include <MViewCreator>

MExtendingBackgroundViewPrivate::MExtendingBackgroundViewPrivate() :
    q_ptr(0), controller(0)
{
}

MExtendingBackgroundViewPrivate::~MExtendingBackgroundViewPrivate()
{
}

void MExtendingBackgroundViewPrivate::init(MWidgetController *controller)
{
    this->controller = controller;
}

QRect MExtendingBackgroundViewPrivate::backgroundGeometry() const
{
    Q_Q(const MExtendingBackgroundView);

    // Calculate background size
    int x = 0;
    int y = 0;
    int width = q->geometry().width();
    int height = q->geometry().height();

    // Get the borders of the scalable background image
    int left = 0, right = 0, top = 0, bottom = 0;
    const MScalableImage *bg = q->style()->backgroundImage();
    if (bg != NULL) {
        bg->borders(&left, &right, &top, &bottom);
    }

    // If the background should extend beyond a border recalculate offset and size
    if (q->style()->extendDirection() == "left") {
        x -= q->geometry().x() + left;
        width += q->geometry().x() + left;
    } else if (q->style()->extendDirection() == "right") {
        width = MApplication::activeWindow()->visibleSceneSize().width() - q->geometry().x() + right;
    } else if (q->style()->extendDirection() == "top") {
        y -= q->geometry().y() + top;
        height += q->geometry().y() + top;
    } else if (q->style()->extendDirection() == "bottom") {
        height = MApplication::activeWindow()->visibleSceneSize().height() - q->geometry().y() + bottom;
    }

    return QRect(x, y, width, height);
}


MExtendingBackgroundView::MExtendingBackgroundView(MWidgetController *controller) :
    MWidgetView(controller),
    d_ptr(new MExtendingBackgroundViewPrivate)
{
    Q_D(MExtendingBackgroundView);
    d->q_ptr = this;
    d->init(controller);
}

MExtendingBackgroundView::MExtendingBackgroundView(MExtendingBackgroundViewPrivate &dd, MWidgetController *controller) :
    MWidgetView(controller),
    d_ptr(&dd)
{
    Q_D(MExtendingBackgroundView);
    d->q_ptr = this;
    d->init(controller);
}

MExtendingBackgroundView::~MExtendingBackgroundView()
{
    delete d_ptr;
}

void MExtendingBackgroundView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    Q_D(const MExtendingBackgroundView);

    const MScalableImage *bg = style()->backgroundImage();
    if (bg != NULL) {
        // Draw the background
        painter->setOpacity(d->controller->effectiveOpacity());
        bg->draw(d->backgroundGeometry(), painter);
    }
}

void MExtendingBackgroundView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't pass mouse events through
    event->accept();
}

void MExtendingBackgroundView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't pass mouse events through
    event->accept();
}

QRectF MExtendingBackgroundView::boundingRect() const
{
    Q_D(const MExtendingBackgroundView);

    return d->backgroundGeometry();
}

M_REGISTER_VIEW_NEW(MExtendingBackgroundView, MWidgetController)
