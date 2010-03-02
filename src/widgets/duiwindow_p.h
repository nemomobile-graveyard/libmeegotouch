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

#ifndef DUIWINDOW_P_H
#define DUIWINDOW_P_H

#include <QTimeLine>
#include "duiwindow.h"

#include <duiscenemanager.h>

class DuiScene;
class DuiOnDisplayChangeEvent;
class DuiGLWidget;

class DuiWindowPrivate
{
public:

    DuiWindowPrivate();
    virtual ~DuiWindowPrivate();

    void handleApplicationLayoutDirectionChangeEvent(QGraphicsItem *item);
    void handleLanguageChangeEvent(QGraphicsItem *item);
    Dui::Orientation orientation(Dui::OrientationAngle angle) const;

#ifdef Q_WS_X11
    void appendVisibilityChangeMask();
#endif

    DuiGLWidget *glWidget;

    Dui::OrientationAngle angle;

    DuiSceneManager *sceneManager;

    void _q_sendOrientationChangedSignal();
    void notifyWidgetsAboutOrientationChange();

    Dui::Orientation oldOrientation;
    bool orientationAngleLocked;
    bool orientationLocked;

    void doEnterDisplayEvent();
    void doExitDisplayEvent();

    void propagateDuiOnDisplayChangeEventToScene(DuiOnDisplayChangeEvent *event);

    bool onDisplay;
    bool onDisplaySet;


protected:
    DuiWindow *q_ptr;
private:
    void init();
    Q_DECLARE_PUBLIC(DuiWindow)

};



#endif
