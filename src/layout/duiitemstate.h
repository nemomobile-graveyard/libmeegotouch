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

#ifndef DUIITEMSTATE_H
#define DUIITEMSTATE_H

#include "duiexport.h"

#include <QtGui/QGraphicsLayout>

class DuiItemStatePrivate;

/*!
 * \brief Information on the items stored inside the layout.
 *
 * Class to hold additional information on the QGraphicsLayoutItems
 * this layout is working on.
 */
class DUI_EXPORT DuiItemState
{
public:
    /*!
     * \brief Flags
     *
     * These flags are giving details on the state of the item in
     * the layout.
     */
    enum Flag {
        /* Nothing special. */
        STATE_FLAG_NONE = 0, //!< If none of the other flags are set, this indicates that the item is not being shown
        /* The item is still doing its remove-animation. */
        STATE_FLAG_TO_BE_DELETED = 1, //!< Indicates that the item will be deleted as soon as its hidden/
        STATE_FLAG_TO_BE_SHOWN = 2,   //!< Indicates that setTargetGeometry has been called for an item that wasn't SHOWING.  The animator will move the item into its starting position and clear this flag.  This is not set during any showing animation.
        STATE_FLAG_TO_BE_HIDDEN = 4,  //!< Indicates that hide() has been called for an item was SHOWING.  The animator will set the hiding animation.  Both this flag and the SHOWING flag will be left set during any hiding animation.  One the animation is finished, the item will be hidden and both this flag and SHOWING will be cleared.  If TO_BE_DELETED is set, the item is deleted.
        STATE_FLAG_SHOWING = 8  //!< Whether the item is currently visible by the user
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    /*!
     * \brief  Constructs an item state.
     *
     * Returns a Null DuiItemState item.
     */
    DuiItemState();
    /*!
     * \brief Constructs an item state.
     *
     * Constructs an DuiItemState item from the values provided. The
     * geometry settings are taken from the geometry of the item.
     *
     * @param item  The QGraphicsLayoutItem.
     */
    explicit DuiItemState(QGraphicsLayoutItem *item);
    /* Copy constructor */
    DuiItemState(const DuiItemState &);
    /* Assignment operator */
    DuiItemState *operator =(const DuiItemState &);

    /*!
     * \brief Equality operator.
     *
     * This operator checks whether both items refer to the
     * same QGraphicsLayoutItem. It does not check any additional
     * information stored in the DuiItemState.
     *
     * @param other The item to compare with this one.
     *
     * @return true if both DuiItemStates refer to the same object and
     *         false otherwise.
     */
    bool operator ==(const DuiItemState &other) const;

    /*!
     * \brief Destructs an item state.
     */
    ~DuiItemState();

    /*!
     * \brief Checks whether this is a null DuiItemState item?
     *
     * @return true if this DuiItemState item has a null QGraphicsLayoutItem
     *              and false otherwise.
     */
    bool isNull() const;

    /*!
     * \brief Getter for the QGraphicsLayoutItem.
     */
    QGraphicsLayoutItem *item() const;

    /*!
     * \brief Getter for the source position.
     */
    QRectF sourceGeometry() const;

    /*!
     * \brief Getter for the target position.
     */
    QRectF targetGeometry() const;

    /*!
     * \brief Getter for the state flags.
     */
    Flags flags() const;

    /*!
     * \brief Set a new target position.
     *
     * This will set the target geometry to the given rectangle, and set
     * source geometry to be the current item geometry
     */
    void setTargetGeometry(const QRectF &rect);

    /*!
     * \brief Set the current position, restarting the animation
     *
     * This will set the item's geometry to the rectangle and set the source
     * geometry, restarting the animation.
     */
    void setGeometry(const QRectF &rect);

    /*!
     * \brief set a new target opacity.
     *
     * This will set the target opacity, between 0 (invisible) to 1.  If it
     * is zero, the item will have its visibility set to false at the end
     * of the animation.  If it is not set to 0 or -1, the item will be set
     * to visible at the start of the animation.
     *
     * Note that QGraphicLayoutItems do not have an opacity, so only their children
     * that are QGraphicsItems will be changed.
     *
     * If this is set to -1, the opacity will not be changed.
     */
    void setTargetOpacity(qreal opacity);

    /*!
     * \brief Get the target opacity
     *
     * This is between 0 (invisible) to 1 (fully visible), or -1 to leave untouched.
     *
     * All child items, visible or not, will have their opacities changed to the
     * target opacity if the target opacity is not -1.
     */
    qreal targetOpacity() const;

    /*!
     * \brief Get the source opacity
     *
     * This is set when setTargetOpacity is called.  It is set to the item with
     * the highest opacity (Each child item has its own opacity).  Child items
     * not visible are treated as having an opacity of 0.  This returns 0 if
     * there were no visible items and -1 if setTargetOpacity has not been called.
     *
     * @return -1 if setTargetOpacity has not been called, or between 0 and 1
     */
    qreal sourceOpacity() const;

    /*!
     * \brief Get the current opacity
     *
     * This looks at the item() and iterates over all the children that are
     * QGraphicsItems and returns the highest opacity of the children.  Note
     * that the children can have different opacities.
     *
     * The opacity of children that are not visible is not considered.
     *
     * An opacity of 0 is returned if no visible items are found
     */
    qreal currentOpacity() const;

    /*!
     * \brief Set the opacity for the item and all of its children
     *
     * This looks at the item() and iterates over all the children that are
     * QGraphicsItem and sets their opacity.
     *
     * If this is set to 0, the items have their visibility set to false
     *
     * For convenience, if @p opacity = -1, nothing is done.
     */
    void setCurrentOpacity(qreal opacity);

    /*!
     * \brief Set new state flags.
     */
    void setFlags(Flags new_flags);

    /*!
     * \brief Is the flag set?
     */
    bool isSet(Flag) const;

    /*!
     * \brief Sets additional flags to the state, returns the updated state
     */
    void addFlags(Flags);

    /*!
     * \brief Removes flags from the state, returns the updated state
     */
    void removeFlags(Flags);

    void hide();

    /*!
     * \brief Current geometry progress for this item
     *
     * The progress starts at 0 and finishes at 1 and indicates how
     * far into the geometry animation we are
     * If this is set to -1, the geometry will not be changed.
     */
    qreal geometryProgress() const;

    /*!
     * \brief Current opacity progress for this item
     *
     * The progress starts at 0 and finishes at 1 and indicates how
     * far into the opacity animation we are
     */
    qreal opacityProgress() const;

    /*!
     * \brief Set the current geometry progress for this item
     *
     * The geometry progress starts at 0 and finishes at 1 and indicates how
     * far into a geometry change animation we are. The animator will use the
     * progress and the sourceGeometry and targetGeometry to set the item
     * geometry.
     *
     * The animator will use this value to set the geometry
     */
    void setGeometryProgress(qreal progress);

    /*!
     * \brief Set the current opacity progress for this item
     *
     * The progress starts at 0 and finishes at 1 and indicates how
     * far into an opacity change animation we are
     *
     * The animator will use this value to set the opacity
     */
    void setOpacityProgress(qreal progress);

    /*!
     * \brief Returns true if the all geometries and flags are identical.
     */
    bool hasIdenticalLayout(const DuiItemState &) const;

    /** \brief Whether the animation is completed
     *
     *  Equivalent to geometryProgress() == 1 && opacityProgress == 1
     */
    bool isAnimationDone() const;
    void animationDone();

protected:
    DuiItemStatePrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(DuiItemState)
};


Q_DECLARE_OPERATORS_FOR_FLAGS(DuiItemState::Flags)

#endif
