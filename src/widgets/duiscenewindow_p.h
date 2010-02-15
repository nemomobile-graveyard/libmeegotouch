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

#ifndef DUISCENEWINDOW_P_H
#define DUISCENEWINDOW_P_H

#include "duiscenewindow.h"
#include "private/duiwidgetcontroller_p.h"

class DuiWindow;
class DuiSceneLayerEffect;

class DuiSceneWindowPrivate : public DuiWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(DuiSceneWindow)

public:
    DuiSceneWindowPrivate();

    // helper function for the public appear() and appearNow()
    // since both are very similar
    void appear(bool now, DuiWindow *window, DuiSceneWindow::DeletionPolicy policy);

    // helper function for the public dismiss() and dismissNow()
    bool dismiss(bool now);

    DuiSceneWindow::WindowType      windowType;
    DuiSceneWindow::DeletionPolicy  policy;

    Qt::Alignment alignment;
    QPointF offset;

    bool managedManually;
    bool shown;
    bool dismissed;

    DuiSceneLayerEffect *effect;
};


#endif
