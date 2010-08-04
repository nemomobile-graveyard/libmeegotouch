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

#ifndef MSCENEMANAGER_P_H
#define MSCENEMANAGER_P_H

#include <QList>
#include <QPointer>

#include "mnamespace.h"
#include "mscenemanager.h"
#include "mscenewindow.h"
#include <mscenewindowevent_p.h>
#include "mpageswitchanimation.h"

class MInputWidgetRelocator;
class MScene;
class MSceneWindow;
class MSceneLayerEffect;
class MSceneManager;
class MNavigationBar;
class MOrientationAnimation;
class MPageSwitchAnimation;
class MWindow;
class MSceneEventEater;

class QAbstractAnimation;
class QGraphicsWidget;
class QTimeLine;
class QPointF;

class MSceneWindowTransition {
    public:
        QPointer<MSceneWindow> sceneWindow;

        enum TransitionType {
            AppearTransition,
            DisappearTransition
        } type;

        // Makes sense only for AppearTransition types.
        MSceneWindow::DeletionPolicy policy;

        bool animated;

        bool operator==(const MSceneWindowTransition &other) const
        {
            return (sceneWindow == other.sceneWindow &&
                    type == other.type &&
                    policy == other.policy &&
                    animated == other.animated);
        }
};

class MSceneManagerPrivate
{
    Q_DECLARE_PUBLIC(MSceneManager)

public:

    void init(MScene *scene);
    virtual ~MSceneManagerPrivate();

    enum WindowTypeZ {
        PlainSceneWindow = 0,
        ApplicationPage = PlainSceneWindow,          // MApplicationPage
        ApplicationMenu = 500,                    // MApplicationMenu
        NavigationBar = 1000,                        // MNavigationBar
        DockWidget = NavigationBar,                  // MDockWidget
        EscapeButtonPanel = 2000,                    // MEscapeButtonPanel
        Dialog = 3000,                               // MDialog
        MessageBox = Dialog,                         // MMessageBox
        ModalSceneWindow = Dialog,                   // MModalSceneWindow
        PopupList = 4000,                            // MPopupList
        Overlay = 6000,                              // MOverlay
        Completer = Overlay,                         // MCompleter
        HomeButtonPanel = 7000,                      // MHomeButtonPanel
        ObjectMenu = 7500,
        NotificationInformation = 8000,              // MNotification - Information
        NotificationEvent = NotificationInformation, // MNotification - Event
        StatusBar = 9000                             // MStatusBar
    };

    void initOrientationAngles();
    int zForWindowType(MSceneWindow::WindowType windowType);
    MSceneLayerEffect *createLayerEffectForWindow(MSceneWindow *window);

    M::Orientation orientation(M::OrientationAngle angle) const;

    QPointF calculateSceneWindowPosition(MSceneWindow *window);
    void setSceneWindowGeometry(MSceneWindow *window);
    void setSceneWindowGeometries();
    void setParentItemForSceneWindow(MSceneWindow *window, MSceneWindow::WindowType type);
    void notifyWidgetsAboutOrientationChange();
    void rotateToAngle(M::OrientationAngle newAngle);
    void setOrientationAngleWithoutAnimation(M::OrientationAngle newAngle);

    void addSceneWindow(MSceneWindow *sceneWindow);
    void removeSceneWindow(MSceneWindow *sceneWindow);
    void addUnmanagedSceneWindow(MSceneWindow *sceneWindow);

    bool isOnDisplay();
    void produceMustBeResolvedDisplayEvent(MSceneWindow *sceneWindow);
    void produceFullyOffDisplayEvents(QGraphicsItem *item);
    void produceSceneWindowEvent(QEvent::Type type, MSceneWindow *sceneWindow,
                                 bool animatedTransition);

    void freezeUIForAnimationDuration(QAbstractAnimation *animation);

    void pushPage(MSceneWindow *page, bool animatedTransition);
    void popPage(bool animatedTransition);
    void setCurrentPage(QPointer<MSceneWindow> page);

    void prepareWindowShow(MSceneWindow *window);

    void appearSceneWindow(MSceneWindow *window,
                      MSceneWindow::DeletionPolicy policy,
                      bool animatedTransition);

    void prepareWindowHide(MSceneWindow *window);

    void disappearSceneWindow(MSceneWindow *window,
                         bool animatedTransition);

    void startPageSwitchAnimation(MSceneWindow *newPage,
                                  MSceneWindow *oldPage,
                                  MPageSwitchAnimation::PageTransitionDirection direction);

    void createOrientationAnimation();

    void createAppearanceAnimationForSceneWindow(MSceneWindow *sceneWindow);
    void createDisappearanceAnimationForSceneWindow(MSceneWindow *sceneWindow);

    bool verifySceneWindowStateBeforeAppear(
            MSceneWindow *sceneWindow,
            MSceneWindow::DeletionPolicy policy,
            bool animatedTransition);

    bool verifySceneWindowStateBeforeDisappear(
            MSceneWindow *sceneWindow,
            bool animatedTransition);

    void applySceneWindowTransition(MSceneWindowTransition *transition);
    void applySceneWindowTransitionsFromPageSwitchQueue();
    void removeSceneWindowFromTransitionQueue(MSceneWindow *sceneWindow,
            QList<MSceneWindowTransition> &transitionQueue);

    void setSceneWindowState(MSceneWindow *sceneWindow,
            MSceneWindow::SceneWindowState newState);

    void onSceneWindowEnteringAppearingState(MSceneWindow *sceneWindow);
    void onSceneWindowEnteringAppearedState(MSceneWindow *sceneWindow);
    void onSceneWindowEnteringDisappearingState(MSceneWindow *sceneWindow);
    void onSceneWindowEnteringDisappearedState(MSceneWindow *sceneWindow);

    void fastForwardSceneWindowTransitionAnimation(MSceneWindow *sceneWindow);

    /*
        Dislocates the scene window from its proper position
     */
    void _q_dislocateSceneWindow(MSceneWindow *sceneWindow, const QPointF &displacement);

    /*
        Returns the scene window back to its proper position
     */
    void _q_undoSceneWindowDislocation(MSceneWindow *sceneWindow);

    void _q_setSenderGeometry();
    void _q_changeGlobalOrientationAngle();
    void _q_emitOrientationChangeFinished();
    void _q_pageShowAnimationFinished();
    void _q_onSceneWindowAppearanceAnimationFinished();
    void _q_onSceneWindowDisappearanceAnimationFinished();
    void _q_onPageSwitchAnimationFinished();

    void _q_unFreezeUI();

    void _q_applySceneWindowTransitionsQueuedDueToOrientationAnimation();

    void _q_triggerAsyncPendingOrientationChange();

    void _q_applyPendingOrientationChange();
#ifdef Q_WS_X11
    void _q_updateDecoratorButtonsProperty();
#endif

public:

    MScene *scene;

    QGraphicsWidget *rootElement;
    QGraphicsWidget *homeButtonRootElement;
    QGraphicsWidget *navigationBarRootElement;

    MOrientationAnimation *orientationAnimation;
    MPageSwitchAnimation *pageSwitchAnimation;

    QList<MSceneWindow *> windows;
    QList<MSceneWindow *> blockingWindows;
    QList< QPointer<MSceneWindow> > pageHistory;

    M::OrientationAngle angle;
    M::OrientationAngle newAngle;

    QPointer<MSceneWindow> currentPage;

    QPointer<MInputWidgetRelocator> inputWidgetRelocator;

    QPointer<MSceneWindow> alteredSceneWindow;
    QPoint sceneWindowTranslation;

    QPointer<MSceneWindow> statusBar;

    // Queued scene window transitions to be applied after an ongoing orientation
    // animation finishes.
    QList<MSceneWindowTransition> queuedTransitionsOrientationAnimation;

    // Queue of page transitions to be applied after an ongoing page switch
    // animation finishes. Usually only one is picked after each page switch
    // animation since it will probably result in anoter page switch animation.
    QList<MSceneWindowTransition> queuedTransitionsPageSwitchAnimation;

    // If an animation gets started by MSceneManager::setOrientationAngle() while
    // another orientation change transition is running, the orientation change
    // request gets remembered as a pending one:
    struct Rotation {
        M::OrientationAngle angle;
        MSceneManager::TransitionMode mode;
    };
    Rotation* pendingRotation;

    QPointer<QObject> debugInterface;

    MSceneManager *q_ptr;

    MSceneEventEater *eventEater;
};

class MSceneManagerTestInterface : public QObject
{
    Q_OBJECT
public:
    MSceneManagerTestInterface(MSceneManagerPrivate *d, QObject *parent = 0);
public Q_SLOTS:
    void fastForwardPageSwitchAnimation();
    void fastForwardSceneWindowTransitionAnimation(MSceneWindow *sceneWindow);
    void addSceneWindow(MSceneWindow *sceneWindow);
    void removeSceneWindow(MSceneWindow *sceneWindow);

public:
    MSceneManagerPrivate *d;
};

#endif
