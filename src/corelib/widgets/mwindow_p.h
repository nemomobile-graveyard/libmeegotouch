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
    bool debugShortcutModifiersPresent(Qt::KeyboardModifiers modifiers) const;

#ifdef Q_WS_X11
    void appendVisibilityChangeMask();
#endif
    
    void _q_onPixmapRequestsFinished();

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

    void initGLViewport();
    void initSoftwareViewport();
    void configureViewport();

    void playScreenshotEffect();

    bool onDisplay;
    bool onDisplaySet;
    
    // If set to true, a call to MWindow::setVisible(true) will delay the
    // showing of the window, until all pixmap requests of the theme have
    // been finished. This prevents a flickering of the application on startup
    // (see MWindow::setVisible() and _q_onPixmapRequestsFinished()).    
    bool delayVisibility;

protected:
    MWindow *q_ptr;
private:
    void init();
    Q_DECLARE_PUBLIC(MWindow)

};



#endif
