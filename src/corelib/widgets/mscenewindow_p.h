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

class MWindow;
class MSceneLayerEffect;
class MAbstractWidgetAnimation;

class MSceneWindowPrivate : public MWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(MSceneWindow)

public:
    MSceneWindowPrivate();

    MSceneWindow::WindowType      windowType;
    MSceneWindow::DeletionPolicy  policy;

    Qt::Alignment alignment;
    QPointF offset;

    bool managedManually;
    bool shown;
    bool dismissed;

    MSceneLayerEffect *effect;

    MAbstractWidgetAnimation *appearanceAnimation;
    MAbstractWidgetAnimation *disappearanceAnimation;
};


#endif
