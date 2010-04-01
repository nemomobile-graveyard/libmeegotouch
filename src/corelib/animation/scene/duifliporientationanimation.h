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

#ifndef DUIFLIPORIENTATIONANIMATION_H
#define DUIFLIPORIENTATIONANIMATION_H

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include <duiorientationanimation.h>
#include <duianimationstyle.h>

class DuiFlipOrientationAnimationPrivate;
class DuiWindow;

/*!
  \class DuiFlipOrientationAnimation
 */
class DuiFlipOrientationAnimation : public DuiOrientationAnimation
{
    Q_OBJECT
    DUI_ANIMATION(DuiAnimationStyle)
    Q_DECLARE_PRIVATE(DuiFlipOrientationAnimation)

    Q_PRIVATE_SLOT(d_func(), void finishedPhase0())
public:

    /*!
      \brief Constructs the orientation animation.
     */
    DuiFlipOrientationAnimation(QObject *parent = NULL);

    /*!
      \brief Destroys the orientation animation.
     */
    virtual ~DuiFlipOrientationAnimation();

    //! \reimp
    // from DuiOrientationAnimation
    virtual void addSceneWindow(DuiSceneWindow *window);
    virtual void removeSceneWindow(DuiSceneWindow *window);
    virtual void setTargetRotationAngle(Dui::OrientationAngle start, Dui::OrientationAngle end);
    //! \reimp_end

protected:
    //! \reimp
    // from DuiOrientationAnimation
    virtual void rootElementChanged();
    //! \reimp_end
};

#endif
#endif

