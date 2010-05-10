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

// Make doxygen skip this internal class
//! \internal
#ifndef MBASICORIENTATIONANIMATION_H
#define MBASICORIENTATIONANIMATION_H

#include <morientationanimation.h>
#include <mbasicorientationanimationstyle.h>
#include <QSize>
#include <mnamespace.h>

class MBasicOrientationAnimationPrivate;
class MSceneWindow;
class QGraphicsWidget;
class QAnimationGroup;

/*!
  \class MBasicOrientationAnimation
  \brief MBasicOrientationAnimation class provides default 3-phased orientation animation.

  Phase 0: Navigation bar & dock widget slide out, scene windows fade out
  Phase 1: Screen is rotated
  Phase 2: Navigation bar & dock widget slide in, scene window fade in
 */
class MBasicOrientationAnimation : public MOrientationAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MBasicOrientationAnimation)
    M_ANIMATION_GROUP(MBasicOrientationAnimationStyle)

public:

    /*!
      \brief Constructs the orientation animation.
     */
    MBasicOrientationAnimation(const QSize &landscapeScreenSize, QObject *parent = NULL);

    /*!
      \brief Destroys the orientation animation.
     */
    virtual ~MBasicOrientationAnimation();

    //! \reimp
    // from MOrientationAnimation
    virtual void addSceneWindow(MSceneWindow *window);
    virtual void removeSceneWindow(MSceneWindow *window);
    virtual void setTargetRotationAngle(M::OrientationAngle start, M::OrientationAngle end);
    //! \reimp_end

protected:
    //! \reimp
    // from MOrientationAnimation
    virtual void rootElementChanged();
    //! \reimp_end

    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_onPhase0Finished())
    Q_PRIVATE_SLOT(d_func(), void _q_onPhase1Finished())
    Q_PRIVATE_SLOT(d_func(), void _q_onPhase2Finished())
};

#endif
//! \internal_end
