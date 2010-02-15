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

#ifndef DUILAYOUTANIMATION_H
#define DUILAYOUTANIMATION_H

#include "duianimation.h"
#include "duiexport.h"
#include "duiitemstate.h"
#include "duilayout.h"
class DuiLayoutAnimationPrivate;

/*!
 * \class DuiLayoutAnimation
 * \brief Animation interface for animating adding and removing of items and animating
 * switching between policies in a DuiLayout.
 *
 * This class provides an base class to be used to animate changes
 * in the DuiLayout. To reduce the memory footprint as well as
 * CPU wakeups this class can be used by several layouts at the same
 * time.
 *
 * The DuiLayout calls the public startAnimation() whenever it sees an item in need
 * for animation. The layout is then added to a list of layouts to animate.
 * This triggers a call to the virtual layoutAnimationStarted() function which is implemented
 * by subclasses.
 *
 * Note that the item to animate will never itself be a layout, as layout items themselves are
 * not animated.
 *
 * \section custom_animation How to write a custom animation
 *
 * The main method to implement for an animator is layoutAnimationStarted(DuiLayout * const layout, DuiItemState *itemstate).
 * The implementing class would typically call DuiItemState::targetGeometry() and
 * DuiItemState::targetOpacity() to determine the target state, then animate the
 * DuiItemState::item() to that final geometry and opacity.
 *
 * The layoutAnimationFinished() is called when all the items in the given layout are
 * flagged as DuiItemState::isAnimationDone()
 * and may be reimplemented to turn off any timers etc.  The default implementation does nothing.
 *
 * The doItemShownAnimation() must be implemented to provide an animation to show
 * the items.  At minimum this should set the item's geometry to something suitable and QGraphicsItem::show() the item.
 *
 * The doItemHiddenAnimation() may also be implemented to provide an animation to hide the item.
 *
 * The implementation must also decide whether the animation for the
 * current item is finished.  When the animation is completed it should call
 * DuiItemState::animationDone().
 */
class DUI_EXPORT DuiLayoutAnimation : public DuiAnimation
{
public:
    /*!
     * \brief Construct the layout animator.
     */
    explicit DuiLayoutAnimation(DuiLayout *layout);
    /*!
     * \brief Destroys the layout animator.
     */
    virtual ~DuiLayoutAnimation();

    /*!
     * \brief Requests animation for an item.
     *
     * This requests animation for the given DuiItemState that is in the given layout.  This in
     * turns calls layoutAnimationStarted().
     *
     * @param layout The layout that the itemstate is in
     * @param itemstate The item that needs animating
     */
    void startAnimation(DuiItemState *itemstate);

    /*!
     * \brief Stop all the animations on this layout immediately.
     */
    void stopAnimation();

    /*!
     * \brief Return whether the animator is currently animating any items.
     */
    bool isAnimating() const;

    /*!
     * \brief Called when an item is been shown.
     *
     * This function is called when an item has been added to the current policy
     * or is being shown.  It can be implemented to provide an animation.
     */
    virtual void doItemShownAnimation(DuiItemState *itemstate) = 0;

    /*!
     * \brief Called when an item is being hidden.
     *
     * This function is called when an item us being hidden (e.g. if the layout switches to a policy
     * that doesn't contain this item or the item is removed from the policy).  It can be
     * implemented to provide an animation.
     *
     * The default implementation does nothing.
     */
    virtual void doItemHiddenAnimation(DuiItemState *itemstate) {
        Q_UNUSED(itemstate)
    };

    /*!
     * \brief Called when an animatedDeleteAt has been called for an item.
     *
     * This function is called when DuiLayout::animatedDeleteAt() is called for an item,
     * It can be implemented to provide an animation.
     *
     * The default implementation calls doItemHiddenAnimation().
     */
    virtual void doItemDeletingAnimation(DuiItemState *itemstate) {
        doItemHiddenAnimation(itemstate);
    }

    /*!
     * \brief Return a pointer to the associated layout
     */
    DuiLayout *layout() const;
protected:
    /*!
     * \brief Notifies derived animator classes whenever an animation is started.
     *
     * You can override this method to maintain internal state.
     *
     * This function will be called for every item in the layout that is animated,
     * each time with a different itemstate.
     *
     * Any changes made to the itemstate will be saved to the layout.  This allows
     * for animating adding, removing, showing and hiding of items.
     *
     * @param layout The layout whose animation is started.
     * @param itemstate A pointer to the item that has begun animating
     */
    virtual void layoutAnimationStarted(DuiItemState *itemstate) = 0;

    /*!
     * \brief Notifies derived animator classes whenever all the animations on a layout have finished.
     *
     * You can override this method to maintain internal state.  The default implementation does nothing.
     *
     * @param layout the layout whose animation is complete.
     */
    virtual void layoutAnimationFinished() {};

    /*! \brief Hide the given item.
     *
     *  Dervived classes should call this function to hide the given item, rather than
     *  calling QGraphicsItem::hide() on the item directly. */
    void hideItemNow(QGraphicsLayoutItem *item);

    /*! \brief Show the given item if it was hidden by the layout.
     *
     *  Dervived classes should call this function to show the given item, rather than
     *  calling QGraphicsItem::show() on the item directly.  */
    void showItemNow(QGraphicsLayoutItem *item);

    /** \internal
     * Shared d_ptr setup constructor. */
    explicit DuiLayoutAnimation(DuiLayoutAnimationPrivate &, DuiLayout *layout);
    /** \internal_end */

    DuiLayoutAnimationPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(DuiLayoutAnimation)
    Q_DECLARE_PRIVATE(DuiLayoutAnimation)
};

#endif // Header Guard
