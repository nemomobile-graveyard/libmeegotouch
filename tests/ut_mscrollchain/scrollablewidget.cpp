/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "scrollablewidget.h"

ScrollableWidget::ScrollableWidget(QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      mContentItem(0)
{
    // Not really needed but illustrates the use of this widget.
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
}
ScrollableWidget::~ScrollableWidget() {}

void ScrollableWidget::setContentItem(QGraphicsItem *item)
{
    delete mContentItem;
    mContentItem = item;
    item->setParentItem(this);
    item->setPos(QPointF());
}

const QGraphicsItem *ScrollableWidget::contentItem() const
{
    return mContentItem;
}

void ScrollableWidget::scrollContents(const QPoint &offset)
{
    if (mContentItem) {
        mContentItem->setPos(mContentItem->pos() + offset);
    }
}

void ScrollableWidget::clearScroll()
{
    if (mContentItem) {
        mContentItem->setPos(QPointF());
    }
}

