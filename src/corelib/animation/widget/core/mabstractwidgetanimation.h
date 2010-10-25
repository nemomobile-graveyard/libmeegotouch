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

#ifndef MABSTRACTWIDGETANIMATION_H
#define MABSTRACTWIDGETANIMATION_H

#include <mabstractwidgetanimationstyle.h>
#include <mparallelanimationgroup.h>

class MAbstractWidgetAnimationPrivate;

/*!
  \class MAbstractWidgetAnimation
  \brief MAbstractWidgetAnimation class is a base class for all widget animations.

 */
class M_CORE_EXPORT MAbstractWidgetAnimation : public MParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MAbstractWidgetAnimation)
    M_ANIMATION_GROUP(MAbstractWidgetAnimationStyle)

protected:
   /*!
        \brief Constructs the widget animation.

        This constructor is meant to be used inside the libmeegotouch to share the
        private data class pointer.
     */
    MAbstractWidgetAnimation(MAbstractWidgetAnimationPrivate *dd, QObject *parent);

public:
    /*!
     * This enum defines the direction of the widget animation.
     */
    enum TransitionDirection {
        In, //!< transitioning into the screen/display
        Out //!< transitioning out of the screen/display
    };

    /*!
        \brief Constructs the widget animation.
     */
    MAbstractWidgetAnimation(QObject *parent = NULL);

    /*!
        \brief Destructs the widget animation.
     */
    virtual ~MAbstractWidgetAnimation();

    /*!
        Restores the properties of the target widget back to their
        original state, before the animation changed them.
     */
    virtual void restoreTargetWidgetState() = 0;

    virtual void setTargetWidget(MWidgetController *widget);

    virtual void setTransitionDirection(MAbstractWidgetAnimation::TransitionDirection direction) = 0;

    MWidgetController *targetWidget();
    const MWidgetController *targetWidget() const;
};

#endif
