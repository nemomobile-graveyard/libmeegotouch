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

#ifndef MITEMSTATE_P_H
#define MITEMSTATE_P_H

#include <QtGui/QGraphicsLayout>

class QGraphicsWidget;

/*!
 * \internal
 * \brief Internal class used by MBasicLayoutAnimation to hold animation information
 *
 */
class MItemState
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
        STATE_FLAG_TO_BE_HIDDEN = 2,  //!< Indicates that hide() has been called for an item was SHOWING.  The animator will set the hiding animation.  Both this flag and the SHOWING flag will be left set during any hiding animation.  One the animation is finished, the item will be hidden and both this flag and SHOWING will be cleared.  If TO_BE_DELETED is set, the item is deleted.
        STATE_FLAG_SHOWING = 4  //!< Whether the item is currently visible by the user
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    /*!
     * \brief Constructs an item state.
     *
     * Constructs an MItemState item from the values provided. The
     * geometry settings are taken from the geometry of the item.
     *
     * @param item  The QGraphicsWidget.
     */
    explicit MItemState(QGraphicsWidget *item);

    /*!
     * \brief Destructs an item state.
     */
    ~MItemState();

    /*!
     * \brief Getter for the QGraphicsWidget.
     */
    QGraphicsWidget *item() const;

    /*!
     * \brief Getter for the starting offset to draw the item
     */
    QPointF sourceTranslate() const;

    /*!
     * \brief Getter for the state flags.
     */
    Flags flags() const;

    /*!
     * \brief Set a new starting offset to draw the item.
     *
     * This will set the starting offset for where to draw the item.
     */
    void setSourceTranslatePoint(const QPointF &translatePoint);

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
    qreal translateProgress() const;

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
    void setTransformProgress(qreal progress);

    /*!
     * \brief Set the current opacity progress for this item
     *
     * The progress starts at 0 and finishes at 1 and indicates how
     * far into an opacity change animation we are
     *
     * The animator will use this value to set the opacity
     */
    void setOpacityProgress(qreal progress);

    /** \brief Whether the animation is completed
     *
     *  Equivalent to translateProgress() == 1 && opacityProgress == 1
     */
    bool isAnimationDone() const;
    void animationDone();

private:
    /*!
     * \brief The item to layout.
     */
    QGraphicsWidget *m_item;

    /*!
     * \brief The original offset to paint the item
     *
     * The item's translatePoint value is animated from this value to the (0,0).
     */
    QPointF m_sourceTranslate;

    qreal m_sourceOpacity;
    qreal m_targetOpacity;

    /*!
     * \brief State of the item
     */
    Flags m_flags;

    /*!
     * \brief Current progress of animation of the geometry
     * Between 0 and 1.  1 indicating finished.
     */
    qreal m_translateProgress;
    /*!
     * \brief Current progress of animation of the opacity
     * Between 0 and 1.  1 indicating finished.
     */
    qreal m_opacityProgress;
    /*! \brief True is the animation has completed */
    bool m_isAnimationDone;
};
/*! \internal_end */

#endif // Header Guard
