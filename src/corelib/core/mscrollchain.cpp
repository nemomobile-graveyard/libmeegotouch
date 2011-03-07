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

#include "mscrollchain.h"
#include "mabstractscroller.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>

MScrollChain::ScrollerDelegateMap MScrollChain::scrollerDelegates;

MScrollChain::MScrollChain(const QGraphicsItem *scrollTarget,
                           const QGraphicsItem *rootItem)
    : rootItem(rootItem),
      scrollTarget(scrollTarget)
{
    if (!scrollTarget
        || (rootItem && !rootItem->isAncestorOf(scrollTarget))) {
        qFatal("MScrollChain: Scroll chain must be created with a valid target widget,"
               " and if root item was given it must be ancestor of the target widget.");
    } else {
        ensureRootItem();
        buildScrollChainItems();
    }
}

MScrollChain::~MScrollChain()
{
    // Restoring chain items is up to user.
}

void MScrollChain::addBottomUpScroll(const QRect &targetRect,
                                     const QPoint &localOriginPoint,
                                     int startingIndex)
{
    for (int i = startingIndex; i < chainItems.count(); ++i) {
        ScrollChainItem &chainItem(chainItems[i]);

        // Map target rectangle and origin point to delegate widget's local coordinates.
        QRect delegateTargetRect = mapToChainItemFromRoot(chainItem, targetRect);
        QPoint delegateOriginPoint = mapToChainItemFromScrollTarget(chainItem, localOriginPoint);

        // Scroll the chain item.
        chainItem.calculateScrolling(delegateTargetRect, delegateOriginPoint);
    }
}

void MScrollChain::addMinimalScroll(const QRect &localRect,
                                    int startingIndex,
                                    int untilIndex)
{
    untilIndex = qBound<int>(0, untilIndex, chainItems.count() - 1);
    startingIndex = qBound<int>(0, startingIndex, untilIndex);

    for (int i = startingIndex; i <= untilIndex; ++i) {
        ScrollChainItem &chainItem(chainItems[i]);

        // Map target rectangle and origin point to delegate widget's local coordinates.
        QRect delegateRect = mapToChainItemFromScrollTarget(chainItem, localRect);

        // Scroll the chain item.
        chainItem.calculateScrolling(delegateRect, delegateRect.topLeft());
    }
}

void MScrollChain::applyScrolling()
{
    for (ChainItemList::iterator item = chainItems.begin(); item != chainItems.end(); ++item) {
        item->applyScrolling();
    }
}

void MScrollChain::stopScrolling()
{
    for (ChainItemList::iterator item = chainItems.begin(); item != chainItems.end(); ++item) {
        item->stopScrolling();
    }
}

void MScrollChain::restore(const MScrollChain *excludeChain)
{
    if (chainItems.isEmpty()) {
        return;
    }

    ChainItemList::iterator commonAncestor = chainItems.end();

    if (excludeChain && excludeChain->count() > 0) {

        for (ChainItemList::iterator item = chainItems.begin(); item != chainItems.end(); ++item) {

            foreach (const ScrollChainItem &excludeItem, excludeChain->chainItems) {
                if (item->widget == excludeItem.widget) {
                    commonAncestor = item;
                    break;
                }
            }

            if (commonAncestor == item) {
                break;
            }
        }
    }

    for (ChainItemList::iterator item = chainItems.begin(); item != commonAncestor; ++item) {
        item->restoreScrolling();
    }
}

int MScrollChain::count() const
{
    return chainItems.count();
}

const QGraphicsWidget *MScrollChain::widgetAt(int index) const
{
    return chainItems.at(index).widget;
}

const QGraphicsItem *MScrollChain::root() const
{
    return rootItem;
}

void MScrollChain::ensureRootItem()
{
    Q_ASSERT(scrollTarget);
    const QGraphicsItem *item = scrollTarget;
    while (item) {
        // First non-identity transformed item is our root element and forms the base
        // for coordinate transformations (translations).
        rootItem = item;

        if (!item->transform().isIdentity()) {
            // Scrolling does not support arbitrary transformations.
            break;
        }
        item = item->parentItem();
    }
}

void MScrollChain::buildScrollChainItems()
{
    QGraphicsWidget *widget = scrollTarget->parentWidget();
    while (widget
           && (static_cast<QGraphicsItem *>(widget) == rootItem
               || rootItem->isAncestorOf(widget))) {

        QSharedPointer<MAbstractScroller> scroller = findScrollerDelegate(widget);
        if (scroller) {
            chainItems.append(ScrollChainItem(scroller, widget));
        }

        widget = widget->parentWidget();
    }
}

QPoint MScrollChain::itemTranslation(const MScrollChain::ScrollChainItem &targetItem) const
{
    // Initialize translation variable with the amount known to QGraphicsItems,
    // i.e. the real offset from root item to target item.
    QPoint translation = rootItem->mapFromItem(targetItem.widget, 0.0, 0.0).toPoint();

    // Chain items add translation not known to QGraphicsItems.
    // Start from end of chain items, i.e. the "root end".
    for (ChainItemList::const_iterator item = chainItems.end() - 1;
         item >= chainItems.begin(); --item) {

        if (item->widget == targetItem.widget) {
            break;
        }
        // Current item's contentsTranslation affects only its children.
        translation += item->contentsTranslation;
    }
    return translation;
}

QRect MScrollChain::mapToChainItemFromRoot(const MScrollChain::ScrollChainItem &item,
                                           const QRect &rect) const
{
    const QPoint translation = itemTranslation(item);
    return QRect(rect.topLeft() - translation, rect.size());
}

QPoint MScrollChain::mapToChainItemFromRoot(const MScrollChain::ScrollChainItem &item,
                                            const QPoint &point) const
{
    const QPoint translation = itemTranslation(item);
    return QPoint(point - translation);
}

QPoint MScrollChain::mapToChainItemFromScrollTarget(const ScrollChainItem &item, const QPoint &point)
{
    // Map the point in a QGraphicsItem way.
    QPoint mappedPoint = item.widget->mapFromItem(scrollTarget, point).toPoint();

    // Add the planned translations.
    foreach (const ScrollChainItem &i, chainItems) {

        // Add the planned offset to current point.
        mappedPoint += i.contentsTranslation;

        if (&i == &item) {
            break;
        }
    }
    return mappedPoint;
}


QRect MScrollChain::mapToChainItemFromScrollTarget(const ScrollChainItem &item,
                                                   const QRect &rect)
{
    QRect moved = rect;
    moved.moveTo(mapToChainItemFromScrollTarget(item, rect.topLeft()));
    return moved;
}

QSharedPointer<MAbstractScroller> MScrollChain::findScrollerDelegate(const QGraphicsWidget *widget) const
{
    QSharedPointer<MAbstractScroller> result;

    // Find closest match.

    const QMetaObject *metaObject = widget->metaObject();

    do {
        if (scrollerDelegates.contains(metaObject)) {
            result = scrollerDelegates[metaObject];
            break;
        }
        metaObject = metaObject->superClass();
    } while (metaObject);

    return result;
}


MScrollChain::ScrollChainItem::ScrollChainItem(QSharedPointer<MAbstractScroller> scroller,
                                               QGraphicsWidget *scrollerWidget)
    : scroller(scroller),
      widget(scrollerWidget)
{
}

void MScrollChain::ScrollChainItem::calculateScrolling(const QRect &targetRect,
                                                       const QPoint &originPoint)
{
    contentsTranslation += scroller->queryScrollingAmount(widget, targetRect, originPoint, contentsTranslation);
}

void MScrollChain::ScrollChainItem::applyScrolling()
{
    scroller->applyScrolling(widget, contentsTranslation);

    // We no longer know what the translation is.
    contentsTranslation = QPoint();
}

void MScrollChain::ScrollChainItem::stopScrolling()
{
    scroller->stopScrolling(widget);
    contentsTranslation = QPoint();
}

void MScrollChain::ScrollChainItem::restoreScrolling()
{
    if (widget) {
        scroller->restoreScrolling(widget);
    }
    contentsTranslation = QPoint();
}
