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

#ifndef MSCENEFADEANIMATION_H
#define MSCENEFADEANIMATION_H

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include <mscenewindowanimation.h>
#include <manimationstyle.h>

class MSceneFadeAnimationPrivate;

/*!
  \class MNavigationbarAnimation
  \brief
 */
class MSceneFadeAnimation : public MSceneWindowAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MSceneFadeAnimation)
    M_ANIMATION(MSceneWindowAnimationStyle)

protected:

    /*!
      \brief Constructs the navigation bar animation.

      This constructor is meant to be used inside the libmeegotouch to share the
      private data class pointer.
     */
    MSceneFadeAnimation(MSceneFadeAnimationPrivate *dd, MSceneWindow *window, QObject *parent = NULL);

public:

    /*!
      \brief Constructs the scene window animation.
     */
    MSceneFadeAnimation(MSceneWindow *window, QObject *parent = NULL);

    /*!
      \brief Sets animation type.
     */
    virtual void setType(MSceneWindowAnimation::Type type, bool useCurrentAsStartValue);
};

#endif
#endif

