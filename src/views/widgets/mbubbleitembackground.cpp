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

#include <QGraphicsSceneMouseEvent>

#include "mbubbleitembackground_p.h"
#include "mwidgetcreator.h"
#include "mtheme.h"
#include "mscalableimage.h"

M_REGISTER_WIDGET(MBubbleItemBackground)

M_REGISTER_VIEW(MBubbleItemBackgroundView, MBubbleItemBackground)

/*
 * View
 */

MBubbleItemBackgroundView::MBubbleItemBackgroundView(MBubbleItemBackground *_controller)
    : MWidgetView(_controller),
    rtl( false )
{
    controller = _controller;
}

void MBubbleItemBackgroundView::drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option) const
{
    Q_UNUSED(option);

    QSizeF currentSize = size();
    if (currentSize.width() == 0 || currentSize.height() == 0)
        return;

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());

    if( controller->messageType() == MBubbleItem::Incoming ) {
        if( !rtl ) {
            if( style()->incomingBackground() )
                style()->incomingBackground()->draw(0, 0, currentSize.width(), currentSize.height(), painter);
        } else {
            if( style()->incomingMirroredBackground() )
                style()->incomingMirroredBackground()->draw(0, 0, currentSize.width(), currentSize.height(), painter);
        }

    } else if ( controller->messageType() == MBubbleItem::Outgoing ) {
        if( !rtl ) {
            if( style()->outgoingBackground() )
                style()->outgoingBackground()->draw(0, 0, currentSize.width(), currentSize.height(), painter);
        } else {
            if( style()->outgoingMirroredBackground() )
                style()->outgoingMirroredBackground()->draw(0, 0, currentSize.width(), currentSize.height(), painter);
        }
    }

    painter->setOpacity(oldOpacity);
}

void MBubbleItemBackgroundView::applyStyle()
{
    MWidgetView::applyStyle();

    if( controller->layoutDirection() == Qt::RightToLeft )
        rtl = true;
    else
        rtl = false;
}

bool MBubbleItemBackgroundView::event(QEvent* event)
{
    if ( event->type() == QEvent::LayoutDirectionChange ) {
        if(rtl)
            rtl = false;
        else
            rtl = true;
    }
    return MWidgetView::event(event);
}

/*
 * Controller
 */

MBubbleItemBackground::MBubbleItemBackground(QGraphicsItem * parent)
    : MWidgetController(parent),
    _messageType(MBubbleItem::Incoming)
{
    setView(new MBubbleItemBackgroundView(this));
}

void MBubbleItemBackground::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    style().setModePressed();
    update();
}

void MBubbleItemBackground::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();
    emit clicked();
    update();
}

void MBubbleItemBackground::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();
    emit canceled();
    update();
}

