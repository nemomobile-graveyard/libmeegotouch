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

#ifndef MWIDGETSLIDEOUTANIMATION_H
#define MWIDGETSLIDEOUTANIMATION_H

#include <mabstractwidgetanimation.h>
#include <mwidgetslideoutanimationstyle.h>

class MWidgetSlideOutAnimationPrivate;

//! \internal
class MWidgetSlideOutAnimation : public MAbstractWidgetAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MWidgetSlideOutAnimation)
    M_ANIMATION_GROUP(MWidgetSlideOutAnimationStyle)

    /*!
      \brief Constructs the widget animation.

      This constructor is meant to be used inside the libmeegotouch to share the
      private data class pointer.
     */
    MWidgetSlideOutAnimation(MWidgetSlideOutAnimationPrivate *dd, QObject *parent);

public:

    /*!
      \brief Constructs the widget animation.
     */
    MWidgetSlideOutAnimation(QObject *parent = NULL);

    /*!
      \brief Destructs the widget animation.
     */
    virtual ~MWidgetSlideOutAnimation();

    //! \reimp
    virtual void restoreTargetWidgetState();
    virtual void setTargetWidget(MWidgetController *widget);
    //! \reimp_end

protected:
    //! \reimp
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    //! \reimp_end
};
//! \internal_end

#endif
