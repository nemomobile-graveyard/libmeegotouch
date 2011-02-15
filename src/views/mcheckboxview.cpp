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

#include "mcheckboxview.h"
#include "mcheckboxview_p.h"

#include "mscalableimage.h"
#include "mviewconstants.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "mbuttontransition.h"

#include "mdebug.h"


MCheckboxViewPrivate::MCheckboxViewPrivate()
    : MButtonViewPrivate()
{
}

MCheckboxViewPrivate::~MCheckboxViewPrivate()
{
}

MCheckboxView::MCheckboxView(MButton *controller) :
    MButtonView(* new MCheckboxViewPrivate, controller)
{
}

MCheckboxView::~MCheckboxView()
{
}

void MCheckboxView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    if (style()->checkmarkImage() && model()->checked()) {
        QSizeF pos = (size() / 2) - (style()->checkmarkImage()->size() / 2);
        painter->drawPixmap(QRectF(QPointF(pos.width(), pos.height()), size()), *style()->checkmarkImage(),
                            QRectF(QPointF(0,0), style()->checkmarkImage()->size()));
    }
}

void MCheckboxView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MCheckboxView);

    if (model()->down()) {
        return;
    }

    // Honor MWidgetView's style and play press feedback.
    style()->pressFeedback().play();

    if (model()->checked()) {
        style()->pressOffFeedback().play();
    } else {
        style()->pressOnFeedback().play();
    }

    model()->setDown(true);
    d->transition->onPress();
}

void MCheckboxView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MCheckboxView);

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);

    if (rect.contains(touch)) {
        if (!model()->down()) {
            model()->setDown(true);
	    d->transition->onPress();

            // Honor MWidgetView's style and play press feedback.
            style()->pressFeedback().play();

            if (model()->checked()) {
                style()->pressOffFeedback().play();
            } else {
                style()->pressOnFeedback().play();
            }
        }
    } else {
        if (model()->down()) {
            model()->setDown(false);
            style()->cancelFeedback().play();
	    d->transition->onCancel();
        }
    }

}

void MCheckboxView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MCheckboxView);

    if (!model()->down()) {
        return;
    }
    model()->setDown(false);
    d->transition->onRelease();

    if (model()->checked()) {
        style()->releaseOffFeedback().play();
    } else {
        style()->releaseOnFeedback().play();
    }

    // Honor MWidgetView's style and play release feedback.
    style()->releaseFeedback().play();

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    if (rect.contains(touch))
        model()->click();
}

M_REGISTER_VIEW_NEW(MCheckboxView, MButton)
