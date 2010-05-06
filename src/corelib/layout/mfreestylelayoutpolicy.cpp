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

#include "mfreestylelayoutpolicy.h"
#include "mfreestylelayoutpolicy_p.h"
#include <QtGui/QWidget>
MFreestyleLayoutPolicy::MFreestyleLayoutPolicy(MLayout *layout) :
    MAbstractLayoutPolicy(*(new MFreestyleLayoutPolicyPrivate(layout)))
{ }

MFreestyleLayoutPolicy::~MFreestyleLayoutPolicy()
{ }

void MFreestyleLayoutPolicy::addItem(QGraphicsLayoutItem *item)
{
    addItemAtGeometry(item, QRectF(contentsArea().topLeft(), item->effectiveSizeHint(Qt::PreferredSize)));
}

void MFreestyleLayoutPolicy::addItemAtGeometry(QGraphicsLayoutItem *item, const QRectF &geom)
{
    Q_D(MFreestyleLayoutPolicy);

    MAbstractLayoutPolicy::addItem(item);

    QPointF topLeft = contentsArea().topLeft();

    //set the item's geometry to geom, bounded by the item's minimum and maximum
    //size.
    QSizeF maximum = item->effectiveSizeHint(Qt::MaximumSize);
    QSizeF minimum = item->effectiveSizeHint(Qt::MinimumSize);
    QRectF new_geometry = QRectF(geom.topLeft(), geom.size().boundedTo(maximum).expandedTo(minimum));

    QList<QRectF> new_geometries;
    const int size = count();
    for (int i = 0; i < size - 1; ++i)
        new_geometries << itemGeometry(i);

    //Add the new item to the list of geometries and place it
    new_geometries << new_geometry;
    d->placeItem(size - 1, new_geometries, topLeft, contentsArea().width());

    for (int i = 0; i < size; ++i)
        setItemGeometry(i, new_geometries.at(i));

    updateGeometry();
}

void MFreestyleLayoutPolicy::moveItemTo(QGraphicsLayoutItem *item, const QRectF &geom)
{
    Q_D(MFreestyleLayoutPolicy);

    int index = indexOf(item);
    if (index < 0)
        return; //Item not found

    QPointF topLeft = contentsArea().topLeft();
    QList<QRectF> new_geometries;

    const int size = count();
    for (int i = 0; i < size; ++i)
        new_geometries << itemGeometry(i);

    new_geometries[index] = geom;

    d->placeItem(index, new_geometries, topLeft, contentsArea().width());

    for (int i = 0; i < size; ++i)
        setItemGeometry(i, new_geometries.at(i));
    updateGeometry();
}

void MFreestyleLayoutPolicy::relayout()
{
    Q_D(MFreestyleLayoutPolicy);

    QPointF topLeft = contentsArea().topLeft();

    QList<QRectF> new_geometries;
    const int size = count();
    for (int i = 0; i < size; ++i) {
        new_geometries << itemGeometry(i);
        d->placeItem(i, new_geometries, topLeft, contentsArea().width());
    }

    QRectF area = contentsArea();
    for (int i = 0; i < size; ++i) {
        area = area.united(new_geometries.at(i));
        setItemGeometry(i, new_geometries.at(i));
    }

    //if we have moved items outside of the bounding box, we need to invalidate the layout
    //causing another relayout, but we have to be careful that we don't end up in an
    //infinite loop because of this.
    if (area.isValid() && !contentsArea().adjusted(-0.0000001, -0.0000001, 0.0000001, 0.0000001).contains(area)) //adjust for rounding errors
        updateGeometry();
}

void MFreestyleLayoutPolicy::removeAt(int index)
{
    MAbstractLayoutPolicy::removeAt(index);
    //since we've removed an item, our size hint would have changed.  invalidate our preferred size etc
    updateGeometry();
}

QSizeF MFreestyleLayoutPolicy::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MFreestyleLayoutPolicy);

    if (which == Qt::MaximumSize) {
        // maximum size is the QWIDGETSIZE_MAX
        QSizeF new_size = constraint;
        if (new_size.width() < 0)
            new_size.setWidth(QWIDGETSIZE_MAX);
        if (new_size.height() < 0)
            new_size.setHeight(QWIDGETSIZE_MAX);
        return new_size;
    }

    // minimum and preferred size of a layout is the bounding box of the children
    int i = count();
    QRectF boundingBox;
    while (--i >= 0) {
        // iterate through children
        boundingBox = boundingBox.united(itemGeometry(i));
    }

    qreal right, bottom;
    d->layout->getContentsMargins(NULL, NULL, &right, &bottom);
    return QSizeF(boundingBox.right() + right - d->layout->geometry().left(), boundingBox.bottom() + bottom - d->layout->geometry().top());
}


