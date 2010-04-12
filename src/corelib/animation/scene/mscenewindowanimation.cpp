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

#include "mscenewindowanimation.h"

#if QT_VERSION >= 0x040600

#include "mscenewindowanimation_p.h"

#include <QPropertyAnimation>
#include <QAnimationGroup>
#include <QGraphicsWidget>


MSceneWindowAnimation::MSceneWindowAnimation(MSceneWindowAnimationPrivate *dd,
        MSceneWindow *window,
        QObject *parent,
        MGroupAnimation::Type type) :
    MGroupAnimation(dd, type, parent)
{
    Q_D(MSceneWindowAnimation);

    d->sceneWindow = window;
}

MSceneWindowAnimation::MSceneWindowAnimation(MSceneWindow *window,
        QObject *parent,
        MGroupAnimation::Type type) :
    MGroupAnimation(new MSceneWindowAnimationPrivate, type, parent)
{
    Q_D(MSceneWindowAnimation);

    d->sceneWindow = window;
}

MSceneWindowAnimation::~MSceneWindowAnimation()
{
}

void MSceneWindowAnimation::updateState(QAbstractAnimation::State oldState,
        QAbstractAnimation::State newState)
{
    MGroupAnimation::updateState(oldState, newState);

    if (newState == QAbstractAnimation::Stopped) {
        Q_D(MSceneWindowAnimation);
        emit animationDone(d->sceneWindow);
    }
}
#endif

