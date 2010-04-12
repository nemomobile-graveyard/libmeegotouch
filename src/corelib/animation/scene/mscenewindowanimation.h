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

#ifndef MSCENEWINDOWANIMATION_H
#define MSCENEWINDOWANIMATION_H

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include <MGroupAnimation>
#include <QPair>
#include <QVector>
#include <mscenewindowanimationstyle.h>

class MSceneWindow;
class MSceneWindowAnimationPrivate;

/*!
  \class MSceneWindowAnimation
  \brief
 */
class MSceneWindowAnimation : public MGroupAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MSceneWindowAnimation)
    M_ANIMATION(MSceneWindowAnimationStyle)

protected:

    /*!
      \brief Constructs the scene window animation.

      This constructor is meant to be used inside the libmeegotouch to share the
      private data class pointer.
     */
    MSceneWindowAnimation(MSceneWindowAnimationPrivate *dd,
                            MSceneWindow *window,
                            QObject *parent = NULL,
                            MGroupAnimation::Type type = Parallel);

public:

    enum Type {
        Show,
        Hide,
        Move
    };

    /*!
      \brief Constructs the scene window animation.
     */
    MSceneWindowAnimation(MSceneWindow *window,
                            QObject *parent = NULL,
                            MGroupAnimation::Type type = Parallel);

    /*!
      \brief Destructs the scene window animation.
     */
    virtual ~MSceneWindowAnimation();

    /*!
      \brief Sets animation type.
     */
    virtual void setType(Type type, bool useCurrentAsStartValue = true) = 0;

Q_SIGNALS:
    void animationDone(MSceneWindow *window);

protected:
    virtual void updateState(QAbstractAnimation::State oldState, QAbstractAnimation::State newState);
};

#endif
#endif
