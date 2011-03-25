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
#include <QElapsedTimer>
#include <QTimer>
#include "mwindow.h"

#include <mscenemanager.h>

class MScene;
class MOnDisplayChangeEvent;
class MStatusBar;
class QGLWidget;
class QGLContext;
class MWindowBorderDecorator;

class MWindowPrivate
{
public:

    MWindowPrivate();
    virtual ~MWindowPrivate();

    void handleApplicationLayoutDirectionChangeEvent(QGraphicsItem *item);
    void handleLanguageChangeEvent(QGraphicsItem *item);
    void handleWindowStateChangeEvent(QWindowStateChangeEvent *event);
    void handleCloseEvent(QCloseEvent *event);
    bool debugShortcutModifiersPresent(Qt::KeyboardModifiers modifiers) const;
    void setVisible(bool visible);
    void _q_enablePaintUpdates();
    MSceneManager::TransitionMode orientationChangeTransitionMode();
    void ensureOrientationAngleIsUpToDateBeforeShowing();

#ifdef Q_WS_X11
    void appendVisibilityChangeMask();
    void applyStartupWindowBackground();
    void setX11Property(const char *propertyName, qreal value);
    qreal getX11Property(const char *propertyName) const;
    void setX11PrestartProperty(bool set);
    void setX11OrientationAngleProperty(M::OrientationAngle angle);
    void resolveOrientationRules();
    void setX11NotificationPreviewsDisabledProperty(bool set);
    bool getX11NotificationPreviewsDisabledProperty() const;
    void setMeegoX11Properties();
    void sendNetCloseWindow();
    void sendDelayedExitDisplayEvent();

    bool notificationPreviewDisabled;
#endif

    void _q_onPixmapRequestsFinished();

    QGLWidget *glWidget;
    // a non-const pointer to the associated gl context
    QGLContext *glContext;

    M::OrientationAngle angle;

    MSceneManager *sceneManager;

    void notifyWidgetsAboutOrientationChange();

    M::Orientation oldOrientation;
    bool orientationAngleLocked;
    bool orientationLocked;

    bool isLogicallyClosed;
    bool isInSwitcher;
    bool closeOnLazyShutdown;

    void doEnterDisplayEvent();
    void doExitDisplayEvent();
    void doSwitcherEntered();
    void doSwitcherExited();
    void sendExitDisplayEvent(bool delayedSending);

    MOnDisplayChangeEvent *delayedMOnDisplayChangeEvent;
    void propagateMOnDisplayChangeEventToScene(MOnDisplayChangeEvent *event);

    void initGLViewport();
    void initSoftwareViewport();
    void configureViewport();
    void disableAutomaticBackgroundRepainting();

    void takeScreenshot();
    void playScreenshotEffect();
    void rotateWindowsFromKeyEvent(QKeyEvent *event);

    /**
      * Calling winId() on a QWidget makes this widget a native window.
      * As the meego graphicssystem does not support native windows in native
      * windows we should not call winId() but effectiveWinId() in MWindow.
      * effectiveWinId() return the winId of the first native parent window.
      * When we are not visible on the screen yet effectiveWinId() does always
      * work and will simply return 0 in the worst case. Therefor we manually
      * search for the topmost parent widget and return its winId.
      * Use this method instead of effectiveWinId() when you are not sure if
      * your window is visible yet.
      */
    WId robustEffectiveWinId() const;

    void _q_exitDisplayStabilized();

    bool onDisplay;
    bool onDisplaySet;
    QTimer displayExitedTimer;

    bool visibleInSwitcher;
    bool fullyObscured;

    QElapsedTimer timeSinceLastPaintInSwitcher;
    bool updateIsPending;
    bool discardedPaintEvent;
    bool beforeFirstPaintEvent;
    int invisiblePaintCounter;
    const int allowedPaintEventsWhenInvisible;
    MWindowBorderDecorator *borderDecorator;

protected:
    MWindow *q_ptr;
private:
    void init();
    Q_DECLARE_PUBLIC(MWindow)

};

#endif
