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

#include "mpannableviewportlayout.h"

#include <QGraphicsWidget>

MPannableViewportLayout::MPannableViewportLayout(QGraphicsLayoutItem *parent)
    : QGraphicsLinearLayout(parent),
      pannedWidget(0),
      directions(0),
      panningPos(QPointF())
{
}

void MPannableViewportLayout::setPanningDirections(const Qt::Orientations &newDirections)
{
    directions = newDirections;
    invalidate();
}

Qt::Orientations MPannableViewportLayout::panningDirections() const
{
    return directions;
}

void MPannableViewportLayout::setWidget(QGraphicsWidget *widget)
{
    if (pannedWidget) {
        removeAt(0);
    }

    pannedWidget = widget;

    if (widget) {
        addItem(widget);
    }
}

void MPannableViewportLayout::setPanningPosition(const QPointF &newPos)
{
    if (newPos != panningPos) {
        panningPos = newPos;
        pannedWidget->setPos(panningPos);
    }
}

void MPannableViewportLayout::setGeometry(const QRectF &rect)
{
    QRectF unboundedRect = rect;
    unboundedRect.moveTo(panningPos);

    if (pannedWidget) {
        switch (directions) {
            case ((int)Qt::Horizontal | (int)Qt::Vertical):
                unboundedRect.setSize(effectiveSizeHint(Qt::PreferredSize).expandedTo(rect.size()));
                break;
            case Qt::Vertical: {
                qreal height = qMax(rect.height(), effectiveSizeHint(Qt::PreferredSize, QSizeF(rect.width(), -1)).height());
                unboundedRect.setHeight(height);
                break;
            }
            case Qt::Horizontal: {
                qreal width = qMax(rect.width(), effectiveSizeHint(Qt::PreferredSize, QSizeF(-1, rect.height())).width());
                unboundedRect.setWidth(width);
                break;
            }
        }
    }

    QGraphicsLinearLayout::setGeometry(unboundedRect);
}
