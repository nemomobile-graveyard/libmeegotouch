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

#ifndef MBASICLAYOUTANIMATION_P_H
#define MBASICLAYOUTANIMATION_P_H

#include "mbasiclayoutanimation.h"
#include "mlayoutanimation_p.h"
#include "mitemstate_p.h"

#include <QElapsedTimer>

/** Private animator class. */
class MBasicLayoutAnimationPrivate : public MLayoutAnimationPrivate
{
    Q_DECLARE_PUBLIC(MBasicLayoutAnimation)
public:
    /** Constructor */
    MBasicLayoutAnimationPrivate();
    /** Destructor */
    virtual ~MBasicLayoutAnimationPrivate();

    /** Trigger animation step. */
    void tick();

    /*! \brief This provides an animation to shown an item.
     *
     * This sets the item's current geometry to its minimum size, centering
     * it on its MItemState::targetGeometry().  This provides an animation
     * of the item 'growing' to its final size.
     * */
    void doItemShownAnimation(MItemState *itemstate);

    /*! \brief This provides an animation to hide an item.
     *
     * This sets the MItemState::targetGeometry() to the items minimum size, centering
     * on its current position.  This provides an animation of the item 'shrinking' before being
     * removed.
     */
    void doItemHiddenAnimation(MItemState *itemstate);
    /*!
     * \brief This method animates an item.
     *
     * This method is doing the actual animation of the objects
     * in the layouts.
     *
     * @param layout The layout containing the item to animate.
     * @param state The info about the layout item to animate.
     */
    void animate(MItemState &state);

    /** Keep track of the time elapsed since the last update.
      * This is used because we may miss some timer events.
      */
    QElapsedTimer timeSinceLastUpdate;
    /** The number of milliseconds elapsed since last update
     *
     * This records the value of timeSinceLastUpdate when an update happens
     */
    qreal recordedTimeSinceLastUpdate;

    QList<MItemState> states;
};

#endif // Header Guard
