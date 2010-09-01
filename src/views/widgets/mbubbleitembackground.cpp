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
    mirrored(false),
    down(false)
{
    controller = _controller;
    connect(controller, SIGNAL(messageTypeChanged()), this, SLOT(refreshStyleMode()));
}

void MBubbleItemBackgroundView::refreshStyleMode()
{
    if (mirrored)
        style().setObjectName(style()->mirroredObjectName());
    else
        style().setObjectName("");

    if (controller->messageType() == MBubbleItem::Outgoing) {
        if (down)
            style().setModeOutgoingPressed();
        else
            style().setModeOutgoing();
    } else if (controller->messageType() == MBubbleItem::Incoming) {
        if (down)
            style().setModeIncomingPressed();
        else
            style().setModeIncoming();
    }

    controller->setContentsMargins(style()->paddingLeft(),
                                   style()->paddingTop(),
                                   style()->paddingRight(),
                                   style()->paddingBottom());

    controller->update();
}

void MBubbleItemBackgroundView::applyStyle()
{
    MWidgetView::applyStyle();

    if( controller->layoutDirection() == Qt::RightToLeft )
        mirrored = true;
    else
        mirrored = false;

    refreshStyleMode();
}

void MBubbleItemBackgroundView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    style()->pressFeedback().play();

    if (!down)
        down = true;

    refreshStyleMode();
}

void MBubbleItemBackgroundView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    style()->releaseFeedback().play();

    if (down) {
        down = false;
        controller->click();
    }

    refreshStyleMode();
}

void MBubbleItemBackgroundView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);

    style()->cancelFeedback().play();

    if (down)
        down = false;

    refreshStyleMode();
}

bool MBubbleItemBackgroundView::event(QEvent* event)
{
    if ( event->type() == QEvent::LayoutDirectionChange ) {
        if(mirrored) {
            mirrored = false;
        } else {
            mirrored = true;
        }
    }

    refreshStyleMode();

    return MWidgetView::event(event);
}

void MBubbleItemBackgroundView::setupModel()
{

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

void MBubbleItemBackground::click()
{
    emit clicked();
}

void MBubbleItemBackground::setMessageType(MBubbleItem::MessageType mt)
{
    _messageType = mt;
    emit messageTypeChanged();
}

MBubbleItem::MessageType MBubbleItemBackground::messageType()
{
    return _messageType;
}
