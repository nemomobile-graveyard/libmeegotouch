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

#ifndef MLAYOUT_P_H
#define MLAYOUT_P_H

#include "mlayout.h"
#include "mlayoutanimation.h"

#include <QtCore/QList>
#include <QtCore/QPointer>
#include <mnamespace.h>

class QGraphicsLayoutItem;
class MLayoutAnimation;

/** Private Layout class.
 */
class MLayoutPrivate
{
public:

    /** Constructor */
    explicit MLayoutPrivate(MLayout *l);

    /*!
     * \brief Method to register a policy with this layout.
     *
     * This is called by MAbstractLayoutPolicy when an instance is created.
     * If there are no other registered policies, this will become the active (current) policy.
     *
     * @param policy  The policy to register.
     */
    void registerPolicy(MAbstractLayoutPolicy *policy);

    /*!
     * \brief Method to unregister a policy with this layout.
     *
     * This is called by MAbstractLayoutPolicy when an instance is destroyed.
     *
     * If this policy was currently active, the most recently created policy will become automatically
     * active.  If there are no policies left, the current policy will become NULL.
     *
     * @param policy  The policy to deregister.
     */
    void unregisterPolicy(MAbstractLayoutPolicy *policy);

    /*! \brief Set the target geometry of the given item to the given geometry.
     *
     *  This is called by MAbstractLayoutPolicy::setGeometry() to set the target
     *  geometry of the item.
     *
     *  Note that this causes the item to be shown, if it's being hidden.
     */
    void setItemGeometry(int index, const QRectF &geometry);

    /*! \brief Return the target geometry of the item at the given index.
     */
    QRectF itemGeometry(int index) const;

    /*! \brief Hide the given item.
     *
     *  Items can be hidden by the policies and are automatically hidden when
     *  switching to a policy not visible.  Items become visible again with a
     *  subsequent call to setItemGeometry().
     *
     *  To hide items as a user of MLayout, see \ref visibility
     */
    void hideItem(int index);

    /** This is called by the animator to trigger cleanup tasks. */
    void animationFinished();
    /** This is called when an item's animation has finished, to delete
     *  the item if it needs deleting */
    void itemAnimationFinished(int index);

    /** Go up the parents and return the first parent that is not a layout */
    QGraphicsItem *parentItem() const;

    /** Hide the given item, setting a flag in MWidget to indicate that the layout hid it */
    void hideItemNow(QGraphicsItem *layoutItem) const;
    /** Show the given item if it was hidden by the layout, clearing the flag in MWidget to indicate that the layout unhid it */
    void showItemNow(QGraphicsItem *layoutItem) const;
    /** Only clear the flag in MWidget */
    void removeHiddenFlag(QGraphicsItem *layoutItem) const;
    /** This is called when the widget's orientation has changed (probably because the device has been rotated) */
    void setOrientation(M::Orientation orientation);
    /** Check the closest parent widget's scene to find out what orientation we are in */
    void recheckOrientation(bool fallbackToActiveWindow = true);
    /** Return the current orientation of the layout */
    M::Orientation orientation() const;

protected:
    Q_DECLARE_PUBLIC(MLayout)
    // Shared d_ptr related code:
    MLayout *q_ptr;
    MAbstractLayoutPolicy *portraitPolicy;
    MAbstractLayoutPolicy *landscapePolicy;

    /** The policy object governing layout. */
    MAbstractLayoutPolicy *current_policy;

    /** The layout animation for this policy */
    MLayoutAnimation *animation;

    /** Policies that are associated with this layout. */
    QList<MAbstractLayoutPolicy *> policies;

    /** The current orientation of the widget that this layout is in */
    M::Orientation m_orientation;

    struct LayoutItem {
        QGraphicsLayoutItem *item;
        QRectF geometry;
        bool toBeDeleted;
    };
    /** List of items */
    QList<LayoutItem> items;
};

#endif // Header Guard
