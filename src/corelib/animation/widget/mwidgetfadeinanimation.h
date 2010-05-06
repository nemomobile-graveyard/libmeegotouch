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

#ifndef MWIDGETFADEINANIMATION_H
#define MWIDGETFADEINANIMATION_H

#include <mabstractwidgetanimation.h>
#include <mwidgetfadeinanimationstyle.h>

class MWidgetFadeInAnimationPrivate;

//! \internal
class MWidgetFadeInAnimation : public MAbstractWidgetAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MWidgetFadeInAnimation)
    M_ANIMATION_GROUP(MWidgetFadeInAnimationStyle)

    /*!
      \brief Constructs the widget animation.

      This constructor is meant to be used inside the libmeegotouch to share the
      private data class pointer.
     */
    MWidgetFadeInAnimation(MWidgetFadeInAnimationPrivate *dd, QObject *parent);

public:

    /*!
      \brief Constructs the widget animation.
     */
    MWidgetFadeInAnimation(QObject *parent = NULL);

    /*!
      \brief Destructs the widget animation.
     */
    virtual ~MWidgetFadeInAnimation();

    //! \reimp
    virtual void restoreTargetWidgetState();
    virtual void setTargetWidget(MWidgetController *widget);
    //! \reimp_end

protected:
    //! \reimp
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    //! \reimp_end
};
//! \internal

#endif
