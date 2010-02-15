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

#ifndef DUILAYOUTHELPER_H
#define DUILAYOUTHELPER_H

#include <QtGui/QGraphicsLayout>
#include <QtGui/QGraphicsItem>

//! \internal

/*!
 * \brief Helper function to operate on all graphics items in and below a layout item at once.
 *
 * This functions iterates over all layouts and sub-layouts to find all graphics items
 * and uses the given function object on all those graphics items.
 * The function object can return a collected value with the value() method.
 *
 * @param  startItem   the layout item to start the iteration at
 * @param  function    the function object whose ()-operator is used on all graphics
 *                     items
 *
 * @return the value returned by the value() method of the given function object
 *
 * @see SetOpacity, GetOpacity
 */
template<class F> typename F::return_type forAllGraphicsItems(QGraphicsLayoutItem *startItem, F function)
{
    QList<QGraphicsLayoutItem *> items;
    items.push_back(startItem);

    while (items.size() > 0) {
        QGraphicsLayoutItem *item = items.front();
        items.pop_front();

        if (0 == item) {
            continue;
        }

        if (item->graphicsItem()) {
            // call the function on graphics item directly
            function(item->graphicsItem());
        } else if (item->isLayout()) {
            // for layouts, queue up all contained child items
            QGraphicsLayout *layout = static_cast<QGraphicsLayout *>(item);

            const int count = layout->count();
            for (int j = 0; j < count; ++j) {
                items.push_back(layout->itemAt(j));
            }
        }
    }

    // the functor might have collected a value, just return it
    return function.value();
}

/*!
 * \brief A base class that is useful to implement function objects for forAllGraphicsItems.
 *
 * It handles the internal value, so that this code does not have to be repeated in
 * every class. If you need a value for the operation you have to specify a constructor
 * that uses the base class constructor with the value given.
 *
 */
template<typename VAL> struct GraphicsItemFunction {
    typedef VAL return_type;

    GraphicsItemFunction(VAL startValue = 0) : m_value(startValue) {}
    return_type value() {
        return m_value;
    }

protected:
    return_type m_value;
};

/*!
 * \brief Function object for forAllGraphicsItems to set the opacity
 */
struct SetOpacity : public GraphicsItemFunction<qreal> {
    SetOpacity(qreal value) : GraphicsItemFunction<qreal>(value) {}
    void operator()(QGraphicsItem *item) {
        Q_ASSERT(item);
        item->setOpacity(m_value);
    }
};

/*!
 * \brief  Function object for forAllGraphicsItems to get the opacity.
 *
 * The collected opacity is the maximum of all opactities of the
 * graphics items.
 */
struct GetOpacity : public GraphicsItemFunction<qreal> {
    void operator()(QGraphicsItem *item) {
        Q_ASSERT(item);
        if (item->isVisible())
            m_value = qMax(item->opacity(), m_value);
    }
};

class ProxyItem : public QGraphicsLayoutItem
{
public:
    ProxyItem(QGraphicsLayoutItem *proxiedItem) {
        item = proxiedItem;
        setOwnedByLayout(true);
    }
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const {
        return item->effectiveSizeHint(which, constraint);
    }
    virtual void getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const {
        item->getContentsMargins(left, top, right, bottom);
    }
    QGraphicsLayoutItem *proxiedItem() const {
        return item;
    }
    void refresh() {
        updateGeometry();
        setSizePolicy(item->sizePolicy());
    }
private:
    QGraphicsLayoutItem *item;
};

//! \internal_end

#endif // Header Guard
