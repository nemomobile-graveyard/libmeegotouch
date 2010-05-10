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

#ifndef MORIENTATIONANIMATION_H
#define MORIENTATIONANIMATION_H

#include <QtGlobal>

#include <mparallelanimationgroup.h>
#include <mnamespace.h>

class QGraphicsWidget;
class MSceneWindow;
class MOrientationAnimationPrivate;

/*!
  \class MOrientationAnimation
  \brief MOrientationAnimation class provides abstract interface for orientation animations.
 */
class MOrientationAnimation : public MParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MOrientationAnimation)
    M_ANIMATION_GROUP(MAnimationStyle)

protected:

    /*!
      \brief Constructs the orientation animation.
     */
    MOrientationAnimation(MOrientationAnimationPrivate *d, QObject *parent);

Q_SIGNALS:
    /*!
     \brief Signals that orientation has changed
    */
    void orientationChanged();

public:

    /*!
      \brief Destroys the orientation animation.
     */
    virtual ~MOrientationAnimation() {};

    /*!
      \brief Sets the root element which contains all scene windows
     */
    void setRootElement(QGraphicsWidget *rootElement);

    /*!
      \brief Add new scene window to animation
    */
    virtual void addSceneWindow(MSceneWindow *window) = 0;

    /*!
      \brief Remove scene window from animation
    */
    virtual void removeSceneWindow(MSceneWindow *window) = 0;

    /*!
      \brief Sets target angle for rotation
    */
    virtual void setTargetRotationAngle(M::OrientationAngle start, M::OrientationAngle end) = 0;

protected:
    /*!
      \brief Returns the root element which contains all scene windows
     */
    QGraphicsWidget *rootElement();

    /*!
      \brief Notification of root element change
     */
    virtual void rootElementChanged();
};

#endif
