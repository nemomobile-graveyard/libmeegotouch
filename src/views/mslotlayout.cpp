/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mslotlayout.h"

MSlotLayout::MSlotLayout(QGraphicsLayoutItem *parent)
    : QGraphicsLayout(parent)
{
    ignoreSizePolicies = false;
}

void MSlotLayout::setWidget(QGraphicsWidget *widget)
{
    QGraphicsWidget *currentWidget = widgetPointer.data();

    if (currentWidget == widget)
        return;

    widgetPointer = widget;

    if (currentWidget) {
        currentWidget->setParentLayoutItem(0);
    }

    if (widget) {
        widget->setParentLayoutItem(this);
        widget->setParentItem(parentItem());
    }

    invalidate();
}


void MSlotLayout::setSizePolicyProxyWidget(QGraphicsWidget *proxyWidget)
{
    QGraphicsWidget *currentProxyWidget = proxyWidgetPointer.data();

    if (currentProxyWidget == proxyWidget)
        return;

    proxyWidgetPointer = proxyWidget;

    invalidate();
}

void MSlotLayout::setIgnoreSizePolicies(bool enabled)
{
    if (ignoreSizePolicies != enabled) {
        ignoreSizePolicies = enabled;
        invalidate();
    }
}

QGraphicsLayoutItem *MSlotLayout::itemAt(int index) const
{
    QGraphicsWidget *widget = widgetPointer.data();
    if (widget && index == 0) {
        return widget;
    } else {
        return 0;
    }
}

void MSlotLayout::removeAt(int index)
{
    if (widgetPointer.data() && index == 0) {
        setWidget(0);
    }
}

int MSlotLayout::count() const
{
    if (widgetPointer.data()) {
        return 1;
    } else {
        return 0;
    }
}

void MSlotLayout::setGeometry(const QRectF &rect)
{
    QGraphicsWidget *widget = widgetPointer.data();
    QGraphicsWidget *sizePolicyProxyWidget = proxyWidgetPointer.data();

    if (widget) {
        qreal newWidth;
        qreal newHeight;
        QSizePolicy::Policy horizontalPolicy;
        QSizePolicy::Policy verticalPolicy;
        // Here we constrain the width as some widgets can have a different size preferrence
        // depending on their maximum allowed width (known as height-for-width).
        // E.g.: a word-wrapping label with a long text would like to have a size of (1000, 30)
        // (all text in a single line). If it knows that its width will be limited to, say, 300,
        // it will break the text in multiple lines therefore ending up with a preferred size
        // of (300, 120).
        QSizeF preferredSize = widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(rect.width(), -1));

        if (ignoreSizePolicies) {
            horizontalPolicy = QSizePolicy::Ignored;
            verticalPolicy = QSizePolicy::Ignored;
        } else if (sizePolicyProxyWidget) {
            horizontalPolicy = sizePolicyProxyWidget->sizePolicy().horizontalPolicy();
            verticalPolicy = sizePolicyProxyWidget->sizePolicy().verticalPolicy();
        } else {
            horizontalPolicy = widget->sizePolicy().horizontalPolicy();
            verticalPolicy = widget->sizePolicy().verticalPolicy();
        }

        newWidth = calculateLength(horizontalPolicy,
                                   preferredSize.width(),
                                   rect.width());

        newHeight = calculateLength(verticalPolicy,
                                    preferredSize.height(),
                                    rect.height());

        widget->setGeometry(0.0f, 0.0f, newWidth, newHeight);
    }

    QGraphicsLayout::setGeometry(rect);
}

qreal MSlotLayout::calculateLength(QSizePolicy::Policy policy, qreal preferred, qreal available)
{
    if ((policy & QSizePolicy::ExpandFlag) ||
        (policy & QSizePolicy::IgnoreFlag)) {

        // strech it to take all available length

        if ((available > preferred && (policy & QSizePolicy::GrowFlag))
            ||
            (available < preferred && (policy & QSizePolicy::ShrinkFlag))) {

            return available;
        } else {
            return preferred;
        }
    } else {
        // use its preferred length
        return preferred;
    }
}

QSizeF MSlotLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QGraphicsWidget *widget = widgetPointer.data();
    if (widget) {
        return widget->effectiveSizeHint(which, constraint);
    } else {
        switch (which) {
            case Qt::MinimumDescent:
            case Qt::MinimumSize:
            case Qt::PreferredSize:
                return QSizeF(0.0f, 0.0f);
            default: // Qt::MaximumSize
                if (constraint.isEmpty())
                    return QSizeF(10000.0f, 10000.0f);
                else
                    return constraint;
        };
    }
}

QGraphicsItem *MSlotLayout::parentItem()
{
    QGraphicsLayoutItem *layoutItem = parentLayoutItem();

    // Jump intermediate layouts
    while (layoutItem && !layoutItem->graphicsItem())
        layoutItem = layoutItem->parentLayoutItem();

    if (layoutItem)
        return layoutItem->graphicsItem();
    else
        return 0;
}