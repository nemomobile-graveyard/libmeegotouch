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

#ifndef MWIDGETZOOMOUTANIMATION_H
#define MWIDGETZOOMOUTANIMATION_H

#include <mabstractwidgetanimation.h>
#include <mwidgetzoomoutanimationstyle.h>

class MWidgetZoomOutAnimationPrivate;

//! \internal
class MWidgetZoomOutAnimation : public MAbstractWidgetAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MWidgetZoomOutAnimation)
    M_WIDGET_ANIMATION(MWidgetZoomOutAnimationStyle, MWidgetView)

    /*!
      \brief Constructs the widget animation.

      This constructor is meant to be used inside the libmeegotouch to share the
      private data class pointer.
     */
    MWidgetZoomOutAnimation(MWidgetZoomOutAnimationPrivate *dd, QObject *parent);

public:

    /*!
      \brief Constructs the widget animation.
     */
    MWidgetZoomOutAnimation(QObject *parent = NULL);

    /*!
      \brief Destructs the widget animation.
     */
    virtual ~MWidgetZoomOutAnimation();

protected:
    //! \reimp
    virtual void updateCurrentTime(int currentTime);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    //! \reimp_end
};
//! \internal_end

#endif
