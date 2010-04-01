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

#ifndef DUILAYOUT_P_H
#define DUILAYOUT_P_H

#include "duilayout.h"
#include "duilayoutanimation.h"

#include <QtCore/QList>
#include <QtCore/QPointer>
#include <duinamespace.h>

class QGraphicsLayoutItem;
class DuiLayoutAnimation;

/** Private Layout class.
 */
class DuiLayoutPrivate
{
public:

    /** Constructor */
    explicit DuiLayoutPrivate(DuiLayout *l);

    /*!
     * \brief Method to register a policy with this layout.
     *
     * This is called by DuiAbstractLayoutPolicy when an instance is created.
     * If there are no other registered policies, this will become the active (current) policy.
     *
     * @param policy  The policy to register.
     */
    void registerPolicy(DuiAbstractLayoutPolicy *policy);

    /*!
     * \brief Method to unregister a policy with this layout.
     *
     * This is called by DuiAbstractLayoutPolicy when an instance is destroyed.
     *
     * If this policy was currently active, the most recently created policy will become automatically
     * active.  If there are no policies left, the current policy will become NULL.
     *
     * @param policy  The policy to deregister.
     */
    void unregisterPolicy(DuiAbstractLayoutPolicy *policy);

    /*! \brief Set the target geometry of the given item to the given geometry.
     *
     *  This is called by DuiAbstractLayoutPolicy::setGeometry() to set the target
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
     *  To hide items as a user of DuiLayout, see \ref visibility
     */
    void hideItem(int index);

    /** This is called by the animator to trigger cleanup tasks. */
    void animationFinished();
    /** This is called when an item's animation has finished, to delete
     *  the item if it needs deleting */
    void itemAnimationFinished(int index);

    /** Go up the parents and return the first parent that is not a layout */
    QGraphicsItem *parentItem() const;

    /** Hide the given item, setting a flag in DuiWidget to indicate that the layout hid it */
    void hideItemNow(QGraphicsItem *layoutItem);
    /** Show the given item if it was hidden by the layout, clearing the flag in DuiWidget to indicate that the layout unhid it */
    void showItemNow(QGraphicsItem *layoutItem);
    /** Only clear the flag in DuiWidget */
    void removeHiddenFlag(QGraphicsLayoutItem *layoutItem);
    /** This is called when the widget's orientation has changed (probably because the device has been rotated) */
    void setOrientation(Dui::Orientation orientation);
    /** Check the closest parent widget's scene to find out what orientation we are in */
    void recheckOrientation();
    /** Return the current orientation of the layout */
    Dui::Orientation orientation() const;

protected:
    Q_DECLARE_PUBLIC(DuiLayout)
    // Shared d_ptr related code:
    DuiLayout *q_ptr;
    DuiAbstractLayoutPolicy *portraitPolicy;
    DuiAbstractLayoutPolicy *landscapePolicy;

    /** The policy object governing layout. */
    DuiAbstractLayoutPolicy *current_policy;

    /** The layout animation for this policy */
    DuiLayoutAnimation *animation;

    /** Policies that are associated with this layout. */
    QList<DuiAbstractLayoutPolicy *> policies;

    /** The current orientation of the widget that this layout is in */
    Dui::Orientation m_orientation;

    struct LayoutItem {
        QGraphicsLayoutItem *item;
        QRectF geometry;
        bool toBeDeleted;
    };
    /** List of items */
    QList<LayoutItem> items;
};

#endif // Header Guard
