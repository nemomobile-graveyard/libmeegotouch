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

#include "mfreestylelayoutpolicy_p.h"

MFreestyleLayoutPolicyPrivate::MFreestyleLayoutPolicyPrivate(MLayout *l) :
    MAbstractLayoutPolicyPrivate(l)
{ }

MFreestyleLayoutPolicyPrivate::~MFreestyleLayoutPolicyPrivate()
{ }

QRectF MFreestyleLayoutPolicyPrivate::moveAway(const QRectF &to_move,
        const QRectF &blocked_area,
        const QPointF &topLeft,
        qreal width) const
{
    QRectF result(to_move);
    QRectF intersection = to_move.intersect(blocked_area);

    bool moved = false;
    if (qAbs(intersection.width()) < qAbs(intersection.height())) {
        // move left or right first
        if (to_move.center().x() < blocked_area.center().x()  && topLeft.x() + to_move.width() <= blocked_area.left()) {
            result.moveRight(blocked_area.left());
            moved = true;
        } else if (blocked_area.right() + result.width() <= topLeft.x() + width) {
            result.moveLeft(blocked_area.right());
            moved = true;
        }
    }

    if (!moved) {
        // otherwise up or down first
        if (to_move.center().y() < blocked_area.center().y() && topLeft.y() + to_move.height() <= blocked_area.top())
            result.moveBottom(blocked_area.top());
        else
            result.moveTop(blocked_area.bottom());
    }

    return result;
}

bool MFreestyleLayoutPolicyPrivate::placeItem(int indexToPlace,
        QList<QRectF> & placed_items,
        const QPointF &topLeft,
        qreal width) const
{
    QRectF &target = placed_items[indexToPlace];
    if (target.right() > topLeft.x() + width)
        target.moveRight(topLeft.x() + width);
    if (target.x() < topLeft.x())
        target.moveLeft(topLeft.x());
    if (target.y() < topLeft.y())
        target.moveTop(topLeft.y());

    const int size = placed_items.count();
    // check all placed items for overlaps
    for (int i = 0; i < size; ++i) {
        if (i != indexToPlace && placed_items.at(i).intersects(target)) {
            // move other out of the way
            QRectF otherTarget = moveAway(placed_items.at(i), target, topLeft, width);

            if (otherTarget == placed_items.at(i))
                break; //No change
            placed_items[i] = otherTarget;

            // place other once more to allow for older items to make room
            placeItem(i, placed_items, topLeft, width);
        }
    }

    // if something is still in the way to new item has to move
    qreal bottom = topLeft.y();
    for (int i = 0; i < size; ++i) {
        const QRectF &other = placed_items.at(i);
        if (i != indexToPlace && other.intersects(target)) // move new item out of the way
            target = moveAway(target, placed_items.at(i), topLeft, width);
        bottom = qMax(other.bottom(), topLeft.y());
    }

    // if there is still some overlap, put new item at the bottom
    for (int i = 0; i < size; ++i) {
        if (i != indexToPlace && placed_items.at(i).intersects(target)) {
            target.moveTop(bottom);
            break;
        }
    }

    // now that everything is moved away, simply place the item
    Q_ASSERT(target.top()  + 0.00001f >= topLeft.y()); //add a small amount for rounding errors
    Q_ASSERT(target.left() + 0.00001f >= topLeft.x());
    Q_ASSERT(target.right() - 0.00001f <= topLeft.x() + width || target.width() > width); //make sure it doesn't go outside the right hand side, unless

    return true;
}

