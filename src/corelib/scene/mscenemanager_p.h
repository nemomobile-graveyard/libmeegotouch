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

class MScene;
class MSceneWindow;
class MSceneLayerEffect;
class MSceneManager;
class MNavigationBar;
class MBasicOrientationAnimation;
class MPageSwitchAnimation;
class MWindow;

class QAbstractAnimation;
class QGraphicsWidget;
class QTimeLine;
class QPointF;

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
        ObjectMenu = 7500,
        ModalSceneWindow = Dialog,                   // MModalSceneWindow
        PopupList = 4000,                            // MPopupList
        NotificationInformation = 5000,              // MNotification - Information
        NotificationEvent = NotificationInformation, // MNotification - Event
        Overlay = 6000,                              // MOverlay
        Completer = Overlay,                         // MCompleter
        HomeButtonPanel = 7000,                      // MHomeButtonPanel
        StatusBar = 8000                             // MStatusBar
    };

    int zForWindowType(MSceneWindow::WindowType windowType);
    MSceneLayerEffect *createLayerEffectForWindow(MSceneWindow *window);

    M::Orientation orientation(M::OrientationAngle angle) const;

    QPointF calculateSceneWindowPosition(MSceneWindow *window);
    void setSceneWindowGeometry(MSceneWindow *window);
    void setSceneWindowGeometries();
    void notifyWidgetsAboutOrientationChange();
    void rotateToAngle(M::OrientationAngle newAngle);
    void setOrientationAngleWithoutAnimation(M::OrientationAngle newAngle);

    void attachWindow(MSceneWindow *window);
    void detachWindow(MSceneWindow *window);

    bool onApplicationPage(QGraphicsItem *item);
    MSceneWindow *parentSceneWindow(QGraphicsItem *item);
    void moveSceneWindow(MSceneWindow *window, int adjustment, int visibleHeight);
    int scrollPageContents(MSceneWindow *window, int adjustment) const;

    bool validateSceneWindowPreAppearanceStatus(MSceneWindow *sceneWindow);

    bool isOnDisplay();
    void produceMustBeResolvedDisplayEvent(MSceneWindow *sceneWindow);
    void produceFullyOffDisplayEvents(QGraphicsItem *item);
    void produceSceneWindowEvent(QEvent::Type type, MSceneWindow *sceneWindow,
                                 bool animatedTransition);

    void freezeUIForAnimationDuration(QAbstractAnimation *animation);

    void pushPage(MSceneWindow *page, bool animatedTransition);
    void popPage(bool animatedTransition);

    void prepareWindowShow(MSceneWindow *window);

    void appearSceneWindow(MSceneWindow *window,
                      MSceneWindow::DeletionPolicy policy,
                      bool animatedTransition);

    void prepareWindowHide(MSceneWindow *window);

    void disappearSceneWindow(MSceneWindow *window,
                         bool animatedTransition);

    void dismissSceneWindow(MSceneWindow *window,
                       bool animatedTransition);

    void startPageSwitchAnimation(MSceneWindow *newPage,
                                  MSceneWindow *oldPage,
                                  MPageSwitchAnimation::PageTransitionDirection direction);

    QRectF calculateAvailableSceneRect(MSceneWindow *window);

    void createAppearanceAnimationForSceneWindow(MSceneWindow *sceneWindow);
    void createDisappearanceAnimationForSceneWindow(MSceneWindow *sceneWindow);

    void _q_setSenderGeometry();
    void _q_changeGlobalOrientation();
    void _q_emitOrientationChangeFinished();
    void _q_pageShowAnimationFinished();
    void _q_onSceneWindowAppeared();
    void _q_onSceneWindowDisappeared();
    void _q_restoreSceneWindow();
    void _q_relocateWindowByInputPanel(const QRect &inputPanelRect);

    //! Prepares current window for SIP.
    void _q_inputPanelOpened();

    //! Restores the current window state after SIP is closed.
    void _q_inputPanelClosed();

    void _q_unFreezeUI();

    void _q_applyQueuedSceneWindowTransitions();

    void _q_triggerAsyncPendingOrientationChange();

    void _q_applyPendingOrientationChange();

public:

    MScene *scene;

    QGraphicsWidget *rootElement;
    MBasicOrientationAnimation *orientationAnimation;
    MPageSwitchAnimation *pageSwitchAnimation;

    QList<MSceneWindow *> *windows;
    QList< QPointer<MSceneWindow> > pageHistory;

    M::OrientationAngle angle;
    M::OrientationAngle newAngle;

    MNavigationBar *navBar;
    MSceneWindow *escapeButtonPanel;
    QPointer<MSceneWindow> currentPage;
    bool navBarHidden;
    bool escapeButtonHidden;

    QPointer<QGraphicsWidget> focusedInputWidget;

    QPointer<MSceneWindow> alteredSceneWindow;
    QPoint sceneWindowTranslation;

    bool pendingSIPClose;

    QPointer<MSceneWindow> statusBar;

    class SceneWindowTransition {
        public:
        MSceneWindow *sceneWindow;

        enum TransitionType {
            AppearTransition,
            DisappearTransition,
            DismissTransition
        } type;

        // Makes sense only for AppearTransition types.
        MSceneWindow::DeletionPolicy policy;

        bool animated;
    };
    QList<SceneWindowTransition> queuedTransitions;

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
};

#endif
