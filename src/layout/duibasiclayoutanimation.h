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

#ifndef DUIBASICLAYOUTANIMATION_H
#define DUIBASICLAYOUTANIMATION_H

#include "duilayoutanimation.h"
#include "duibasiclayoutanimationstyle.h"
#include "duianimation.h"

class DuiBasicLayoutAnimationPrivate;

/*!
 * The DuiBasicLayoutAnimation provides a simple animation of items for a DuiLayout.
 * It animates the items geometry and opacity in a linear way, with every animation
 * taking the same amount of time.
 */
class DUI_EXPORT DuiBasicLayoutAnimation : public DuiLayoutAnimation
{
    DUI_ANIMATION(DuiBasicLayoutAnimationStyle)
public:
    /** \brief Construct a basic layout animator.
     */
    DuiBasicLayoutAnimation(DuiLayout *layout);

    /** \brief Destructor. */
    virtual ~DuiBasicLayoutAnimation();

    /*! \brief This provides an animation to shown an item.
     *
     * This sets the item's current geometry to its minimum size, centering
     * it on its DuiItemState::targetGeometry().  This provides an animation
     * of the item 'growing' to its final size.
     * */
    virtual void doItemShownAnimation(DuiItemState *itemstate);

    /*! \brief This provides an animation to hide an item.
     *
     * This sets the DuiItemState::targetGeometry() to the items minimum size, centering
     * on its current position.  This provides an animation of the item 'shrinking' before being
     * removed.
     */
    virtual void doItemHiddenAnimation(DuiItemState *itemstate);

protected:
    /*!
     * \brief This method animates an item.
     *
     * This method is doing the actual animation of the objects
     * in the layouts.
     *
     * @param layout The layout containing the item to animate.
     * @param state The info about the layout item to animate.
     */
    virtual void animate(DuiItemState &state);

    /*! \reimp */
    virtual void layoutAnimationStarted(DuiItemState *itemstate);
    virtual void layoutAnimationFinished();

    virtual void updateCurrentTime(int msecs);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    virtual int duration() const {
        return -1;
    }
    /*! \reimp_end */

private:
    Q_DISABLE_COPY(DuiBasicLayoutAnimation)
    Q_DECLARE_PRIVATE_D(DuiLayoutAnimation::d_ptr, DuiBasicLayoutAnimation)

};

#endif // Header Guard
