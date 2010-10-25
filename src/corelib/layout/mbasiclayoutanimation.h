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

#ifndef MBASICLAYOUTANIMATION_H
#define MBASICLAYOUTANIMATION_H

#include "mlayoutanimation.h"
#include "mbasiclayoutanimationstyle.h"
#include "manimation.h"

class MBasicLayoutAnimationPrivate;

/*!
 * The MBasicLayoutAnimation provides a simple animation of items for a MLayout.
 * It animates the items geometry and opacity in a linear way, with every animation
 * taking the same amount of time.
 */
class M_CORE_EXPORT MBasicLayoutAnimation : public MLayoutAnimation
{
    M_ANIMATION(MBasicLayoutAnimationStyle)
public:
    /** \brief Construct a basic layout animator.
     */
    MBasicLayoutAnimation(MLayout *layout);

    /** \brief Destructor. */
    virtual ~MBasicLayoutAnimation();

    virtual void itemRemovedFromLayout(int index);
    virtual void itemAddedToLayout(int index);
    virtual void animatedDeleteItem(int index);
    virtual void setItemGeometry(int index, const QRectF &geometry);
    virtual void hideItem(int index);

protected:
    /*! \reimp */
    virtual void updateCurrentTime(int msecs);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    virtual int duration() const {
        return -1;
    }
    /*! \reimp_end */

private:
    Q_DISABLE_COPY(MBasicLayoutAnimation)
    Q_DECLARE_PRIVATE_D(MLayoutAnimation::d_ptr, MBasicLayoutAnimation)

};

#endif // Header Guard
