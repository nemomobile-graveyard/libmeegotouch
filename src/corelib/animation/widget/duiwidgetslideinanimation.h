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

#ifndef DUIWIDGETSLIDEINANIMATION_H
#define DUIWIDGETSLIDEINANIMATION_H

#include <duiabstractwidgetanimation.h>
#include <duiwidgetslideinanimationstyle.h>

class DuiWidgetSlideInAnimationPrivate;

//! \internal
class DuiWidgetSlideInAnimation : public DuiAbstractWidgetAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiWidgetSlideInAnimation)
    DUI_ANIMATION_GROUP(DuiWidgetSlideInAnimationStyle)

    /*!
      \brief Constructs the widget animation.

      This constructor is meant to be used inside the libdui to share the
      private data class pointer.
     */
    DuiWidgetSlideInAnimation(DuiWidgetSlideInAnimationPrivate *dd, QObject *parent);

public:

    /*!
      \brief Constructs the widget animation.
     */
    DuiWidgetSlideInAnimation(QObject *parent = NULL);

    /*!
      \brief Destructs the widget animation.
     */
    virtual ~DuiWidgetSlideInAnimation();

    //! \reimp
    virtual void restoreTargetWidgetState();
    virtual void setTargetWidget(DuiWidgetController *widget);
    //! \reimp_end

protected:
    //! \reimp
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    //! \reimp_end
};
//! \internal_end

#endif
