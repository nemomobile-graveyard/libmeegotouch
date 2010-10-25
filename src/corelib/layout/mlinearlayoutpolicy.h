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

#ifndef MLINEARLAYOUTPOLICY_H
#define MLINEARLAYOUTPOLICY_H

#include "mabstractlayoutpolicy.h"

class MLinearLayoutPolicyPrivate;
class QGraphicsLayoutItem;

/*!
 * \class MLinearLayoutPolicy
 * \brief Policy that uses the Qt grid layout engine to provide a horizontal or vertical linear layout.
 * \ingroup layouts
 *
 * This class provides a policy which is similar to QGraphicsLinearLayout, with the advantage
 * of allowing multiple policies and animation.  You can use QGraphicsLinearLayout instead
 * to slightly reduce memory overhead if these advantages are not required.
 *
 * The following example adds items to the linear layout policy:
 *
 * \dontinclude mlinearlayoutpolicy/mlinearlayoutpolicy.cpp
 * \skip Create a MLayout
 * \until }
 *
 * The result, with appropriate CSS styling, looks like:
 * \image html mlinearlayoutpolicy.jpg
 *
 * \sa \ref layout-mlinearlayoutpolicy
 *
 * \section using_qt_instead Using QGraphicsLinearLayout instead
 *
 * If you do not need animations or multiple policies, you can use QGraphicsLinearLayout for same effect in less code.
 * For example:
 *
 * \dontinclude qgraphicslinearlayout/qgraphicslinearlayout.cpp
 * \skip Create a linear layout
 * \until }
 * \sa \ref layout-qgraphicslinearlayout, \ref layout-qgraphicslayout
 */
class M_CORE_EXPORT MLinearLayoutPolicy : public MAbstractLayoutPolicy
{
public:
    /*!
     * \brief Constructs a linear layout policy.
     *
     * The initial @p orientation is required by the constructor, but can be changed at any time
     * with setOrientation().
     *
     * @param layout The layout to associate with.
     * @param orientation The orientation to use for this layout.
     */
    explicit MLinearLayoutPolicy(MLayout *layout, Qt::Orientation orientation);

    /*!
     * \brief Destroys the linear layout policy.
     */
    virtual ~MLinearLayoutPolicy();

    /*!
     * \brief  Get the spacing.  The spacing is the distance between the items
     *
     * Note that in the CSS file, the attributes are horizontal-spacing and vertical-spacing.
     * The appropriate spacing is used depending on whether the linear layout is horizontal
     * or vertical.
     * The horizontal and vertical spacing can also be set individually with
     * setHorizontalSpacing() and setVerticalSpacing()
     *
     * @return The spacing to use.
     * \sa QGraphicsLinearLayout::spacing()
     */
    qreal spacing() const;

    /*!
     * \brief Set the spacing.  The spacing is the distance between the items
     *
     * \sa QGraphicsLinearLayout::setSpacing()
     */
    virtual void setSpacing(qreal spacing);

    /*!
     * \brief  Set the spacing after the item at @p index.
     *
     * This overrides any spacing set by setSpacing().
     *
     * @param index The position of the item to set the spacing after.
     * @param spacing The spacing to use.
     *
     * \sa itemSpacing(), setSpacing(), spacing(), QGraphicsLinearLayout::setItemSpacing()
     */
    void setItemSpacing(int index, qreal spacing);

    /*!
     * \brief  Get the spacing after the item at the given @p index.
     *
     * This overrides any spacing set by setSpacing().
     *
     * @param index The position of the item to get the spacing after.
     * @return The spacing to use.
     * \sa QGraphicsLinearLayout::itemSpacing()
     */
    qreal itemSpacing(int index) const;

    /*!
     * \brief Returns the layout orientation.
     * \sa setOrientation()
     */
    Qt::Orientation orientation() const;

    /*!
     * \brief Change the layout policy orientation to \a orientation.
     *
     * Changing the policy orientation will automatically invalidate the policy
     * and invalidate the layout if this policy is the current layout policy.
     *
     * \sa orientation()
     */
    void setOrientation(Qt::Orientation orientation);

    /*! \brief Add an item to the end of the linear layout policy.
     *
     * This adds the given item to the end of the linear layout policy, using the default
     * alignment of 0.
     *
     * Equivalent to:
     * \code
     *   insertItem(item, -1);
     * \endcode
     *
     * Note that the order of the items in the policy is independent of the order of the items
     * in the MLayout.
     *
     * @param item The item to add.
     */
    virtual void addItem(QGraphicsLayoutItem *item);

    /*! \brief Add an item to the end of the linear layout policy with the given alignment
     *
     * This adds the given item to the end of the linear layout policy, using the given @p alignment
     *
     * Equivalent to:
     * \code
     *   insertItem(item, -1, alignment);
     * \endcode
     *
     * Note that the order of the items in the policy is independent of the order of the items
     * in the MLayout.
     *
     */
    void addItem(QGraphicsLayoutItem *item, Qt::Alignment alignment);

    /*!
     * \brief Insert an item in the policy at the given index.
     *
     * Inserts @p item into the @p layout at index, or before any item that is currently at @p index.
     *
     * Note that the order of the items in the policy is independent of the order of the items
     * in the MLayout.
     *
     * @param item The item to insert.
     * @param index The index to place the item
     */
    virtual void insertItem(int index, QGraphicsLayoutItem *item);

    /*! \brief Insert an item in the policy at the given index with the given alignment.
     *
     * Inserts @p item into the @p layout at index, or before any item that is currently at @p index.
     *
     * Note that the order of the items in the policy is independent of the order of the items
     * in the MLayout.
     */
    void insertItem(int index, QGraphicsLayoutItem *item, Qt::Alignment alignment);

    /*! \brief Add a stretch with the given \a stretch factor
      *
      * This convenience function is equivalent to calling
      * \code
      *   insertStretch(-1, stretch).
      * \endcode
      *
      * Adding a stretch does not increase the count() of the policy, cannot be refderenced by itemAt(),
      * and cannot be removed.
      *
      */
    inline void addStretch(int stretch = 1) {
        insertStretch(-1, stretch);
    }

    /** \brief Inserts a stretch of \a stretch at \a index, or before any item that is currently at \a index
      *
      * Adding a stretch does not increase the count() of the policy, cannot be refderenced by itemAt(),
      * and cannot be removed.
      *
      * \sa addStretch(), setStretchFactor(), setItemSpacing(), insertItem()
      */
    void insertStretch(int index, int stretch = 1);

    /*! \brief Returns the stretch factor for \a item.
     *
     *  The default stretch factor is 0, meaning that the item has no assigned stretch factor.
     *  \sa setStretchFactor()
     */
    int stretchFactor(QGraphicsLayoutItem *item) const;

    /*! \brief Sets the stretch factor for \a item to \a stretch.
     *
     * If an item's stretch factor changes, this function will invalidate the layout.
     * Setting \a stretch to 0 removes the stretch factor from the item, and is
     * effectively equivalent to setting \a stretch to 1.
     *
     * \sa stretchFactor()
     */
    void setStretchFactor(QGraphicsLayoutItem *item, int stretch);

    /*!
      Returns the alignment for \a item. The default alignment is
      Qt::AlignCenter.

      The alignment decides how the item is positioned within its assigned space
      in the case where there's more space available in the layout than the
      widgets can occupy.

      \sa setAlignment()
     */
    Qt::Alignment alignment(QGraphicsLayoutItem *item) const;
    /*!
      Sets the alignment of \a item to \a alignment. If \a item's alignment
      changes, the layout is automatically invalidated.

      \sa alignment(), invalidate()
     */
    void setAlignment(QGraphicsLayoutItem *item, Qt::Alignment alignment);


    /*! \reimp */
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setHorizontalSpacing(qreal spacing);
    virtual void setVerticalSpacing(qreal spacing);
    virtual void removeAt(int index);
    /*! \reimp_end */

    /*!
      Enable/disable automatic setting of the layout position for the items inside the layout.

      \sa layoutPositioningEnabled(), MWidgetController::setLayoutPosition()
     */
    void setNotifyWidgetsOfLayoutPositionEnabled(bool enabled);

    /*!
      Returns true if positioning is enabled, false if positioning is disabled.

      \sa enableLayoutPositioning()
     */    
    bool isNotifyWidgetsOfLayoutPositionEnabled() const;

protected:
    /*! \reimp */
    virtual void relayout();
    virtual void invalidate();
    virtual void activated();
    /*! \reimp_end */

private:
    Q_DISABLE_COPY(MLinearLayoutPolicy)
    Q_DECLARE_PRIVATE(MLinearLayoutPolicy)
};

#endif // Header Guard
