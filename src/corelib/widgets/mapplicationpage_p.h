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

#ifndef MAPPLICATIONPAGE_P_H
#define MAPPLICATIONPAGE_P_H

#include <mapplicationpage.h>
#include <mscenewindow_p.h>
#include <QRect>
#include <QWeakPointer>

class MPannableViewport;

class MApplicationPagePrivate : public MSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(MApplicationPage)

public:

    MApplicationPagePrivate();
    void init();

    virtual void doEnterDisplayEvent();
    // Note: Called by MApplicationWindow
    void setExposedContentRect(const QRectF &rect);

    MPannableViewport *pannableViewport;
    Qt::Orientations panDirection;

    bool contentCreated;

    QWeakPointer<QGraphicsWidget> customNavigationBarContent;

    void _q_onSceneWindowStateChanged(MSceneWindow::SceneWindowState newState,
                                      MSceneWindow::SceneWindowState oldState);
    void _q_onCentralWidgetDestroyed();
};

#endif
