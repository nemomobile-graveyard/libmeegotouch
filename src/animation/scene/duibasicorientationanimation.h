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

// Make doxygen skip this internal class
//! \internal
#ifndef DUIBASICORIENTATIONANIMATION_H
#define DUIBASICORIENTATIONANIMATION_H

#include <duiparallelanimationgroup.h>
#include <duibasicorientationanimationstyle.h>
#include <QSize>
#include <duinamespace.h>

class DuiBasicOrientationAnimationPrivate;
class DuiSceneWindow;
class QGraphicsWidget;
class QAnimationGroup;

/*!
  \class DuiBasicOrientationAnimation
  \brief DuiBasicOrientationAnimation class provides default 3-phased orientation animation.

  Phase 0: Navigation bar & dock widget slide out, scene windows fade out
  Phase 1: Screen is rotated
  Phase 2: Navigation bar & dock widget slide in, scene window fade in
 */
class DuiBasicOrientationAnimation : public DuiParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiBasicOrientationAnimation)
    DUI_ANIMATION_GROUP(DuiBasicOrientationAnimationStyle)

public:

    /*!
      \brief Constructs the orientation animation.
     */
    DuiBasicOrientationAnimation(const QSize &landscapeScreenSize, QObject *parent = NULL);

    /*!
      \brief Destroys the orientation animation.
     */
    virtual ~DuiBasicOrientationAnimation();

    //! \reimp
    // from DuiOrientationAnimation
    virtual void addSceneWindow(DuiSceneWindow *window);
    virtual void removeSceneWindow(DuiSceneWindow *window);
    virtual void setTargetRotationAngle(Dui::OrientationAngle start, Dui::OrientationAngle end);
    //! \reimp_end

    void setRootElement(QGraphicsWidget *rootElement);

Q_SIGNALS:
    /*!
     \brief Signals that orientation has changed
    */
    void orientationChanged();

protected:
    //! \reimp
    // from DuiOrientationAnimation
    virtual void rootElementChanged();
    //! \reimp_end

    QGraphicsWidget *rootElement();

    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_onPhase0Finished())
    Q_PRIVATE_SLOT(d_func(), void _q_onPhase1Finished())
    Q_PRIVATE_SLOT(d_func(), void _q_onPhase2Finished())
};

#endif
//! \internal_end
