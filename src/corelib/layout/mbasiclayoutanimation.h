/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
 *
 * The details of the animation can be specified in your CSS file.  For example:
 *
 * \code
 *    x-translation-easing-curve: outback;
 *    y-translation-easing-curve: outback;
 *    opacity-easing-curve: linear;
 *    scale-easing-curve: linear;
 *    initial-showing-opacity: 0.0;
 *    final-hiding-opacity: 0.0;
 *
 *    initial-showing-scale: 0.0;
 *    final-hiding-scale: 0.0;
 *
 *    animate-opacity: true;
 *    animate-scale: false;
 *
 *    duration: 600;
 *
 * \endcode
 *
 * When a new item is added to the layout, the QGraphicsItem::opacity() is animated
 * starting from the given initial-showing-opacity to an opacity of 1.0, and the QGraphicsItem::scale() is animated from the
 * initial-showing-scale to 1.0, in a duration of 'duration' milliseconds.  The opacity, scale, and
 * translation animations can use any QEasingCurve, including a meegotouch easing curve such
 * MOvershotBezierEasingCurve.
 *
 * The scale and opacity animations can be disabled entirely by setting animate-opacity and
 * animate-scale to false.
 *
 * If you want the final scale or opacity to be something other than 1.0, you can place the widget
 * inside of another widget, since their opacities will be combined.  Or you can inherit from
 * MBasicLayoutAnimation and customize the animation.  Or disable layout animations and use a
 * QPropertyAnimation.
 *
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
