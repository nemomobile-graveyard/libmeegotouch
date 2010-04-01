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

#include "duiextendingbackgroundview.h"
#include "duiextendingbackgroundview_p.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <DuiWidgetController>
//#include <DuiSceneManager>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiScalableImage>
#include <DuiViewCreator>

DuiExtendingBackgroundViewPrivate::DuiExtendingBackgroundViewPrivate() :
    q_ptr(0), controller(0)
{
}

DuiExtendingBackgroundViewPrivate::~DuiExtendingBackgroundViewPrivate()
{
}

void DuiExtendingBackgroundViewPrivate::init(DuiWidgetController *controller)
{
    this->controller = controller;
}

QRect DuiExtendingBackgroundViewPrivate::backgroundGeometry() const
{
    Q_Q(const DuiExtendingBackgroundView);

    // Calculate background size
    int x = 0;
    int y = 0;
    int width = q->geometry().width();
    int height = q->geometry().height();

    // Get the borders of the scalable background image
    int left = 0, right = 0, top = 0, bottom = 0;
    const DuiScalableImage *bg = q->style()->backgroundImage();
    if (bg != NULL) {
        bg->borders(&left, &right, &top, &bottom);
    }

    // If the background should extend beyond a border recalculate offset and size
    if (q->style()->extendDirection() == "left") {
        x -= q->geometry().x() + left;
        width += q->geometry().x() + left;
    } else if (q->style()->extendDirection() == "right") {
        width = DuiApplication::activeWindow()->visibleSceneSize().width() - q->geometry().x() + right;
    } else if (q->style()->extendDirection() == "top") {
        y -= q->geometry().y() + top;
        height += q->geometry().y() + top;
    } else if (q->style()->extendDirection() == "bottom") {
        height = DuiApplication::activeWindow()->visibleSceneSize().height() - q->geometry().y() + bottom;
    }

    return QRect(x, y, width, height);
}


DuiExtendingBackgroundView::DuiExtendingBackgroundView(DuiWidgetController *controller) :
    DuiWidgetView(controller),
    d_ptr(new DuiExtendingBackgroundViewPrivate)
{
    Q_D(DuiExtendingBackgroundView);
    d->q_ptr = this;
    d->init(controller);
}

DuiExtendingBackgroundView::DuiExtendingBackgroundView(DuiExtendingBackgroundViewPrivate &dd, DuiWidgetController *controller) :
    DuiWidgetView(controller),
    d_ptr(&dd)
{
    Q_D(DuiExtendingBackgroundView);
    d->q_ptr = this;
    d->init(controller);
}

DuiExtendingBackgroundView::~DuiExtendingBackgroundView()
{
    delete d_ptr;
}

void DuiExtendingBackgroundView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    Q_D(const DuiExtendingBackgroundView);

    const DuiScalableImage *bg = style()->backgroundImage();
    if (bg != NULL) {
        // Draw the background
        painter->setOpacity(d->controller->effectiveOpacity());
        bg->draw(d->backgroundGeometry(), painter);
    }
}

void DuiExtendingBackgroundView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't pass mouse events through
    event->accept();
}

void DuiExtendingBackgroundView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't pass mouse events through
    event->accept();
}

QRectF DuiExtendingBackgroundView::boundingRect() const
{
    Q_D(const DuiExtendingBackgroundView);

    return d->backgroundGeometry();
}

DUI_REGISTER_VIEW_NEW(DuiExtendingBackgroundView, DuiWidgetController)
