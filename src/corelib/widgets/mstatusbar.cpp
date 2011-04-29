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
#include "mstatusbar.h"
#include "mstatusbar_p.h"
#include "mdynamicpropertywatcher.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MStatusBar)

MStatusBar::MStatusBar()
    : MSceneWindow(new MStatusBarPrivate, new MSceneWindowModel, MSceneWindow::StatusBar)
{
    Q_D(MStatusBar);
    setObjectName("statusBar");
    d->setupClippedPaintOffsetProperty();
}

MStatusBar::MStatusBar(QGraphicsItem *parent)
    : MSceneWindow(new MStatusBarPrivate, new MSceneWindowModel, MSceneWindow::StatusBar, QString(), parent)
{
    Q_D(MStatusBar);
    setObjectName("statusBar");
    d->setupClippedPaintOffsetProperty();
}

MStatusBar::~MStatusBar()
{
}

MStatusBarPrivate::MStatusBarPrivate() : MSceneWindowPrivate()
{
}

MStatusBarPrivate::~MStatusBarPrivate()
{
}

void MStatusBarPrivate::setupClippedPaintOffsetProperty()
{
    Q_Q(MStatusBar);
    bool ok;

    q->setProperty("clippedPaintOffset", QPointF(0.0f, 0.0f));

    MDynamicPropertyWatcher *pixmapPaintOffsetPropertyWatcher = new MDynamicPropertyWatcher(q);
    pixmapPaintOffsetPropertyWatcher->watch(q);
    pixmapPaintOffsetPropertyWatcher->setPropertyName("clippedPaintOffset");
    ok = q->connect(pixmapPaintOffsetPropertyWatcher, SIGNAL(propertyChanged()),
               SLOT(_q_scheduleUpdate()));
    if (!ok) qFatal("signal connection failed");
}

void MStatusBarPrivate::_q_scheduleUpdate()
{
    Q_Q(MStatusBar);
    q->update();
}

bool MStatusBar::sceneEvent(QEvent *event)
{
    return MSceneWindow::sceneEvent(event);
}

#include "moc_mstatusbar.cpp"