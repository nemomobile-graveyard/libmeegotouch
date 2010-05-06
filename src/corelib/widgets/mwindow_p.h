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

#ifndef MWINDOW_P_H
#define MWINDOW_P_H

#include <QTimeLine>
#include "mwindow.h"

#include <mscenemanager.h>

class MScene;
class MOnDisplayChangeEvent;
class MStatusBar;
class QGLWidget;

class MWindowPrivate
{
public:

    MWindowPrivate();
    virtual ~MWindowPrivate();

    void handleApplicationLayoutDirectionChangeEvent(QGraphicsItem *item);
    void handleLanguageChangeEvent(QGraphicsItem *item);
    M::Orientation orientation(M::OrientationAngle angle) const;

#ifdef Q_WS_X11
    void appendVisibilityChangeMask();
#endif

    QGLWidget *glWidget;

    M::OrientationAngle angle;

    MSceneManager *sceneManager;

    void notifyWidgetsAboutOrientationChange();

    M::Orientation oldOrientation;
    bool orientationAngleLocked;
    bool orientationLocked;

    bool isLogicallyClosed;
    bool closeOnLazyShutdown;

    void doEnterDisplayEvent();
    void doExitDisplayEvent();

    void propagateMOnDisplayChangeEventToScene(MOnDisplayChangeEvent *event);

    bool onDisplay;
    bool onDisplaySet;

protected:
    MWindow *q_ptr;
private:
    void init();
    Q_DECLARE_PUBLIC(MWindow)

};



#endif
