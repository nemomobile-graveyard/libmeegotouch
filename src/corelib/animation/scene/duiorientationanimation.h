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

#ifndef DUIORIENTATIONANIMATION_H
#define DUIORIENTATIONANIMATION_H

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include <duigroupanimation.h>
#include <duigroupanimationstyle.h>
#include <duinamespace.h>

class QGraphicsWidget;
class DuiWindow;
class DuiSceneWindow;
class DuiOrientationAnimationPrivate;

/*!
  \class DuiOrientationAnimation
  \brief DuiOrientationAnimation class provides abstract interface for orientation animations.
 */
class DuiOrientationAnimation : public DuiGroupAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiOrientationAnimation)

    DUI_ANIMATION(DuiGroupAnimationStyle)

protected:

    /*!
      \brief Constructs the orientation animation.
     */
    DuiOrientationAnimation(DuiOrientationAnimationPrivate *d,
                            QObject *parent,
                            DuiGroupAnimation::Type type = DuiGroupAnimation::Parallel);

Q_SIGNALS:
    /*!
     \brief Signals that orientation has changed
    */
    void orientationChanged();

public:

    /*!
      \brief Constructs the orientation animation.
     */
    DuiOrientationAnimation(QObject *parent, DuiGroupAnimation::Type type = DuiGroupAnimation::Parallel);
    /*!
      \brief Destroys the orientation animation.
     */
    virtual ~DuiOrientationAnimation() {};

    /*!
      \brief Sets the root element which contains all scene windows
     */
    void setRootElement(QGraphicsWidget *rootElement);

    /*!
      \brief Add new scene window to animation
    */
    virtual void addSceneWindow(DuiSceneWindow *window) = 0;

    /*!
      \brief Remove scene window from animation
    */
    virtual void removeSceneWindow(DuiSceneWindow *window) = 0;

    /*!
      \brief Sets target angle for rotation
    */
    virtual void setTargetRotationAngle(Dui::OrientationAngle start, Dui::OrientationAngle end) = 0;

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
#endif
