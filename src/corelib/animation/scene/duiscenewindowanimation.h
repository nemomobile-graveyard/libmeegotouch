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

#ifndef DUISCENEWINDOWANIMATION_H
#define DUISCENEWINDOWANIMATION_H

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include <DuiGroupAnimation>
#include <QPair>
#include <QVector>
#include <duiscenewindowanimationstyle.h>

class DuiSceneWindow;
class DuiSceneWindowAnimationPrivate;

/*!
  \class DuiSceneWindowAnimation
  \brief
 */
class DuiSceneWindowAnimation : public DuiGroupAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiSceneWindowAnimation)
    DUI_ANIMATION(DuiSceneWindowAnimationStyle)

protected:

    /*!
      \brief Constructs the scene window animation.

      This constructor is meant to be used inside the libdui to share the
      private data class pointer.
     */
    DuiSceneWindowAnimation(DuiSceneWindowAnimationPrivate *dd,
                            DuiSceneWindow *window,
                            QObject *parent = NULL,
                            DuiGroupAnimation::Type type = Parallel);

public:

    enum Type {
        Show,
        Hide,
        Move
    };

    /*!
      \brief Constructs the scene window animation.
     */
    DuiSceneWindowAnimation(DuiSceneWindow *window,
                            QObject *parent = NULL,
                            DuiGroupAnimation::Type type = Parallel);

    /*!
      \brief Destructs the scene window animation.
     */
    virtual ~DuiSceneWindowAnimation();

    /*!
      \brief Sets animation type.
     */
    virtual void setType(Type type, bool useCurrentAsStartValue = true) = 0;

Q_SIGNALS:
    void animationDone(DuiSceneWindow *window);

protected:
    virtual void updateState(QAbstractAnimation::State oldState, QAbstractAnimation::State newState);
};

#endif
#endif
