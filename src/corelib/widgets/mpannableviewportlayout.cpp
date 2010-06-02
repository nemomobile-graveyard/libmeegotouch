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
      panningDirections(0)
{
}

void MPannableViewportLayout::setPanningDirections(const Qt::Orientations &newDirections)
{
    panningDirections = newDirections;
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

void MPannableViewportLayout::setGeometry(const QRectF &rect)
{
    QRectF unboundedRect = rect;

    if (pannedWidget) {
        if (panningDirections.testFlag(Qt::Horizontal)) {
            qreal width = qMax(rect.width(), pannedWidget->effectiveSizeHint(Qt::PreferredSize).width());
            unboundedRect.setWidth(width);
        }

        if (panningDirections.testFlag(Qt::Vertical)) {
            qreal height = qMax(rect.height(), pannedWidget->effectiveSizeHint(Qt::PreferredSize).height());
            unboundedRect.setHeight(height);
        }
    }

    QGraphicsLinearLayout::setGeometry(unboundedRect);
}
