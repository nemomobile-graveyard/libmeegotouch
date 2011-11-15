/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef MCROSSFADEDORIENTATIONANIMATION_H
#define MCROSSFADEDORIENTATIONANIMATION_H

#include "morientationanimation.h"
#include <mcrossfadedorientationanimationstyle.h>

class MCrossFadedOrientationAnimationPrivate;

class MCrossFadedOrientationAnimation : public MOrientationAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MCrossFadedOrientationAnimation)
    M_ANIMATION_GROUP(MCrossFadedOrientationAnimationStyle)

public:
    /*!
     * \brief Constructor
     *
     * \param visibleSceneRect The rectangle of the scene that is rendered by MWindow.
     *                         Equals to the bounding rectangle of the root element in
     *                         scene coordinates.
     */
    MCrossFadedOrientationAnimation(const QRectF &visibleSceneRect, QObject *parent = 0);

    /*!
     * \brief Destructor
     */
    virtual ~MCrossFadedOrientationAnimation();

    virtual void addSceneWindow(MSceneWindow *window);
    virtual void removeSceneWindow(MSceneWindow *window);
    virtual void setTargetRotationAngle(M::OrientationAngle start, M::OrientationAngle end);

protected:
    //! \reimp
    // from MOrientationAnimation
    virtual void rootElementChanged();
    //! \reimp_end

    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    void updateCurrentTime(int currentTime);

private:
    void rotateRootElement(M::OrientationAngle orientation);

    Q_PRIVATE_SLOT(d_func(), void _q_onGeometryChanged())
};

#endif
//! \internal_end
