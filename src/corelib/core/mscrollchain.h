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

#ifndef MSCROLLCHAIN_H
#define MSCROLLCHAIN_H

#include <QList>
#include <QMap>
#include <QPoint>
#include <QPointer>
#include <QRect>
#include <QSharedPointer>

class QGraphicsItem;
class QGraphicsWidget;
class QGraphicsScene;
class MAbstractScroller;

//! \internal

/*!
 *  \brief Performs scrolling operations to move target widget to a wanted position.
 *
 *  The Chain is comprised of scroller widgets that do the actual work. To use this
 *  class, an \a MAbstractScroller derived class must be registered for the widget
 *  type in question.
 *
 *  After registering the scrollers with \a registerScroller(), scrolling can be
 *  planned by calls to \a addBottomUpScroll() and applied by calling \a run(). If widgets
 *  support it, they can be restored by calling \a restore().
 */
class MScrollChain
{
public:
    /*! \brief Constructs a scroll chain.
     *  \param scrollTarget The widget that needs to be scrolled somewhere.
     *  \param rootItem The QGraphicsItem ancestor that marks the end of the chain.
     *                  Target coordinates are given in rootItem coordinates. If not
     *                  given the last suitable ancestor of scroll target is choosed.
     */
    explicit MScrollChain(const QGraphicsItem *scrollTarget,
                          const QGraphicsItem *rootItem = 0);
    virtual ~MScrollChain();

    /*! \brief Scrolls the chain in child-to-parent order.
     *
     *  This method can be called multiple times with different parameters. Doing so
     *  may result in different distribution of the scrolling work, distributed among
     *  the widgets in this chain.
     *
     *  \param targetRect Target location for the target widget in root coordinates. See \a root().
     *  \param originPoint A position inside the target widget to be scrolled to \a targetRect.
     *         Given in target widget own local coordinates.
     *  \param startingIndex Scrolling can optionally be started from a scroller with given index.
     */
    void addBottomUpScroll(const QRect &targetRect,
                           const QPoint &originPoint,
                           int startingIndex = 0);

    /*! \brief Scrolls the chain with minimal change in child-to-parent order.
     *
     *  Calling this ensures that the given \localRect is made visible with
     *  minimal scrolling.
     */
    void addMinimalScroll(const QRect &localRect,
                          int startingIndex,
                          int untilIndex);

    /*! \brief Applies the planned scrolling
     *
     *  Widgets are not expected to have finished their scrolling when this returns.
     */
    void applyScrolling();

    void stopScrolling();

    /*! \brief Restores the state of scroller widgets.
     *
     *  This is does not necessarily undo the scrolling but can be used to
     *  restore a widget which may have moved away from its desirable position.
     *
     *  \param excludeChain Optional. Defines a chain which is to be left untouched.
     */
    void restore(const MScrollChain *excludeChain = 0);

    //! Number of scrollers in the chain.
    int count() const;

    //! \brief Returns scroller widget with index \a index.
    const QGraphicsWidget *widgetAt(int index) const;

    //! \brief Returns root QGraphicsItem used to form the base coordinates.
    const QGraphicsItem *root() const;

    /*! \brief Static method for registering new scrollers
     *
     *  Registered scrollers are available to all MScrollChains. Scrollers registered
     *  for base classes can also be used for derived classes. However, closest match is used.
     *
     *  For example, a scroller for \a QGraphicsWidget is registered by calling:
     *  \codeline MScrollChain::registerScroller<QGraphicsWidget>(QSharedPointer(new MyGraphicsWidgetScroller));
     */
    template <class T> static void registerScroller(QSharedPointer<MAbstractScroller> scroller);

private:
    class ScrollChainItem {
    public:
        // Helper methods
        ScrollChainItem(QSharedPointer<MAbstractScroller> scroller, QGraphicsWidget *scrollerWidget);
        void calculateScrolling(const QRect &targetRect, const QPoint &originPoint);
        void applyScrolling();
        void stopScrolling();
        void restoreScrolling();

        // Scroller for our widget
        QSharedPointer<MAbstractScroller> scroller;

        // Widget whose contents we scroll
        QPointer<QGraphicsWidget> widget;

        // Store the planned translation of the contents.
        QPoint contentsTranslation;
    };

    typedef QList<ScrollChainItem> ChainItemList;

    // Main contents of this scroll chain. There's an item for each widget
    // that is capable of doing scrolling, and which are found between root
    // and scroll target widget.
    ChainItemList chainItems;

    QSharedPointer<MAbstractScroller> findScrollerDelegate(const QGraphicsWidget *widget) const;

    void ensureRootItem();
    void buildScrollChainItems();

    QPoint itemTranslation(const MScrollChain::ScrollChainItem &targetItem) const;
    QRect mapToChainItemFromRoot(const ScrollChainItem &item, const QRect &rect) const;
    QPoint mapToChainItemFromRoot(const ScrollChainItem &item, const QPoint &point) const;
    QPoint mapToChainItemFromScrollTarget(const ScrollChainItem &item, const QPoint &point);
    QRect mapToChainItemFromScrollTarget(const ScrollChainItem &item,
                                         const QRect &rect);

private:
    typedef QMap<const QMetaObject *, QSharedPointer<MAbstractScroller> > ScrollerDelegateMap;

    const QGraphicsItem *rootItem;
    const QGraphicsItem * const scrollTarget;

    static ScrollerDelegateMap scrollerDelegates;

    Q_DISABLE_COPY(MScrollChain)
    MScrollChain();
};

template <class T>
void MScrollChain::registerScroller(QSharedPointer<MAbstractScroller> delegate)
{
    const QMetaObject *metaObject = &T::staticMetaObject;

    if (delegate) {
        ScrollerDelegateMap::iterator iter = scrollerDelegates.find(metaObject);
        if (iter != scrollerDelegates.end()) {
            iter.value() = delegate;
        } else {
            scrollerDelegates[metaObject] = delegate;
        }
    } else {
        scrollerDelegates.remove(metaObject);
    }
}

//! \internal_end

#endif // MSCROLLCHAIN_H
