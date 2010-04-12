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

#ifndef MFLIPORIENTATIONANIMATION_H
#define MFLIPORIENTATIONANIMATION_H

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include <morientationanimation.h>
#include <manimationstyle.h>

class MFlipOrientationAnimationPrivate;
class MWindow;

/*!
  \class MFlipOrientationAnimation
 */
class MFlipOrientationAnimation : public MOrientationAnimation
{
    Q_OBJECT
    M_ANIMATION(MAnimationStyle)
    Q_DECLARE_PRIVATE(MFlipOrientationAnimation)

    Q_PRIVATE_SLOT(d_func(), void finishedPhase0())
public:

    /*!
      \brief Constructs the orientation animation.
     */
    MFlipOrientationAnimation(QObject *parent = NULL);

    /*!
      \brief Destroys the orientation animation.
     */
    virtual ~MFlipOrientationAnimation();

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
};

#endif
#endif

