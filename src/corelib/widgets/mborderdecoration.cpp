/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mborderdecoration.h"
#include "mscenewindow_p.h"

#include <mwidgetcreator.h>
M_REGISTER_WIDGET_NO_CREATE(MBorderDecoration)

MBorderDecoration::MBorderDecoration()
    : MSceneWindow(new MSceneWindowPrivate, new MSceneWindowModel,
                   MSceneWindow::BorderDecoration)
{
    // Presence of a MBorderDecoration should not affect interactions
    // at all (such as grabbing mouse input, gestures, focus), unlike other scene
    // windows. Thus the flags set by MSceneWindow base class should be reverted here.
    setAcceptTouchEvents(false);
    ungrabGesture(Qt::PanGesture);
    ungrabGesture(Qt::TapGesture);
    ungrabGesture(Qt::TapAndHoldGesture);
    ungrabGesture(Qt::PinchGesture);
    ungrabGesture(Qt::SwipeGesture);
    setFlag(QGraphicsItem::ItemStopsClickFocusPropagation, false);
    setFlag(QGraphicsItem::ItemHasNoContents, true);
    setFocusPolicy(Qt::NoFocus);
}

MBorderDecoration::~MBorderDecoration()
{
}
