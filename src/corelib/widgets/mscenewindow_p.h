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

#ifndef MSCENEWINDOW_P_H
#define MSCENEWINDOW_P_H

#include "mscenewindow.h"
#include "private/mwidgetcontroller_p.h"
#include <mabstractwidgetanimation.h>
#include <MSceneManager>
#include <QPointer>

class MWindow;
class MSceneLayerEffect;
class MSceneWindowTransition;

class MSceneWindowPrivate : public MWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(MSceneWindow)

public:
    MSceneWindowPrivate();

    virtual bool canDisappear() {return true;}
    virtual bool canDismiss() {return true;}

    // Called by MSceneManagerPrivate
    void setSceneWindowState(MSceneWindow::SceneWindowState newState);

    // Shoot for the disappear timeout
    void startDisappearTimeout();

    MSceneWindow::WindowType      windowType;
    MSceneWindow::DeletionPolicy  policy;

    Qt::Alignment alignment;
    QPointF offset;

    MSceneWindow::SceneWindowState sceneWindowState;

    bool managedManually;
    bool dismissed;

    MSceneLayerEffect *effect;
    QGraphicsItem *displacementItem;

    QPointer<MAbstractWidgetAnimation> appearanceAnimation;
    QPointer<MAbstractWidgetAnimation> disappearanceAnimation;

    // While appearing a disappear() can be queued and
    // while disappearing an appear() can be queued.
    // Transition to be applied after either appearanceAnimation
    // or disappearanceAnimation finishes.
    MSceneWindowTransition *queuedTransition;

    // Scene manager that owns this scene window.
    QPointer<MSceneManager> sceneManager;

    // Focused item before window blocked.
    QPointer<QGraphicsWidget> focusItemBeforeWindowBlocked;
};

class MSceneWindowTestInterface : public QObject
{
    Q_OBJECT
public:
    MSceneWindowTestInterface(MSceneWindowPrivate *d, QObject *parent = 0);
public Q_SLOTS:
    void setSceneWindowState(MSceneWindow::SceneWindowState newState);

public:
    MSceneWindowPrivate *d;
};

#endif
