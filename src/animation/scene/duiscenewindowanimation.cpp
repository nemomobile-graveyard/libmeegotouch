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

#include "duiscenewindowanimation.h"

#if QT_VERSION >= 0x040600

#include "duiscenewindowanimation_p.h"

#include <QPropertyAnimation>
#include <QAnimationGroup>
#include <QGraphicsWidget>


DuiSceneWindowAnimation::DuiSceneWindowAnimation(DuiSceneWindowAnimationPrivate *dd,
        DuiSceneWindow *window,
        QObject *parent,
        DuiGroupAnimation::Type type) :
    DuiGroupAnimation(dd, type, parent)
{
    Q_D(DuiSceneWindowAnimation);

    d->sceneWindow = window;
}

DuiSceneWindowAnimation::DuiSceneWindowAnimation(DuiSceneWindow *window,
        QObject *parent,
        DuiGroupAnimation::Type type) :
    DuiGroupAnimation(new DuiSceneWindowAnimationPrivate, type, parent)
{
    Q_D(DuiSceneWindowAnimation);

    d->sceneWindow = window;
}

DuiSceneWindowAnimation::~DuiSceneWindowAnimation()
{
}

void DuiSceneWindowAnimation::updateState(QAbstractAnimation::State oldState,
        QAbstractAnimation::State newState)
{
    DuiGroupAnimation::updateState(oldState, newState);

    if (newState == QAbstractAnimation::Stopped) {
        Q_D(DuiSceneWindowAnimation);
        emit animationDone(d->sceneWindow);
    }
}
#endif

