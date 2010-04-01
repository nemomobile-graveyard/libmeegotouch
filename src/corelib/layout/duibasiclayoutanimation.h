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
    Q_DISABLE_COPY(DuiBasicLayoutAnimation)
    Q_DECLARE_PRIVATE_D(DuiLayoutAnimation::d_ptr, DuiBasicLayoutAnimation)

};

#endif // Header Guard
