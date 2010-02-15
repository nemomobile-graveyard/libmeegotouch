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

#ifndef DUIWIDGETZOOMINANIMATION_H
#define DUIWIDGETZOOMINANIMATION_H

#include <duiabstractwidgetanimation.h>
#include <duiwidgetzoominanimationstyle.h>

class DuiWidgetZoomInAnimationPrivate;


//! \internal
class DuiWidgetZoomInAnimation : public DuiAbstractWidgetAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiWidgetZoomInAnimation)
    DUI_WIDGET_ANIMATION(DuiWidgetZoomInAnimationStyle, DuiWidgetView)

    /*!
      \brief Constructs the widget animation.

      This constructor is meant to be used inside the libdui to share the
      private data class pointer.
     */
    DuiWidgetZoomInAnimation(DuiWidgetZoomInAnimationPrivate *dd, QObject *parent);

public:

    /*!
      \brief Constructs the widget animation.
     */
    DuiWidgetZoomInAnimation(QObject *parent = NULL);

    /*!
      \brief Destructs the widget animation.
     */
    virtual ~DuiWidgetZoomInAnimation();

    //! \reimp
    virtual void resetToInitialState();
    //! \reimp_end

protected:
    //! \reimp
    virtual void updateCurrentTime(int currentTime);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    //! \reimp_end
};

//! \internal_end

#endif
