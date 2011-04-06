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
#include "mscenemanagerstyle.h"
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
class MPageSwitchSlideAnimation;
class MContentFadeAndSlideAnimation;
class MWindow;
class MSceneEventEater;

class QAbstractAnimation;
class QGraphicsWidget;
class QTimeLine;
class QPointF;

/*
    List of all scene windows whose state != Disappeared.
    Ordered by Z-value, from lowest to highest.
 */
class MSceneWindowStack {
public:
    void add(MSceneWindow *sceneWindow);
    void remove(MSceneWindow *sceneWindow);
    const QList<MSceneWindow *> &list() {return m_list;}
private:
    QList<MSceneWindow *> m_list;
};

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

    MSceneManagerPrivate();
    virtual ~MSceneManagerPrivate();

    void init(MScene *scene);

    enum WindowTypeZ {
        PlainSceneWindow = 0,
        ApplicationPage = PlainSceneWindow,          // MApplicationPage
        ApplicationMenu = 500,                       // MApplicationMenu
        NavigationBar = 1000,                        // MNavigationBar
        DockWidget = NavigationBar,                  // MDockWidget
        EscapeButtonPanel = 2000,                    // MEscapeButtonPanel
        Sheet = 3000,                                // MSheet
        Dialog = 4000,                               // MDialog
        MessageBox = Dialog,                         // MMessageBox
        ModalSceneWindow = Dialog,                   // MModalSceneWindow
        PopupList = 5000,                            // MPopupList
        Overlay = 6000,                              // MOverlay
        Completer = Overlay,                         // MCompleter
        HomeButtonPanel = 7000,                      // MHomeButtonPanel
        ObjectMenu = 7500,                           // MObjectMenu
        NotificationInformation = 8000,              // MNotification - Information
        NotificationEvent = NotificationInformation, // MNotification - Event
        StatusBar = 9000,                            // MStatusBar
        BorderDecoration = StatusBar                 // MBorderDecoration
    };

    void initOrientationAngles();
    int zForWindowType(MSceneWindow::WindowType windowType);
    void setupLayerEffectForSceneWindow(MSceneWindow *sceneWindow);
    void destroyLayerEffectForSceneWindow(MSceneWindow *sceneWindow);
    bool windowIntersectsRect(const QRectF &rect, MSceneWindow *window);

    QRectF calculateAvailableSceneRect(MSceneWindow *sceneWindow) const;
    QRectF calculateSceneWindowGeometry(MSceneWindow *window) const;
    void setSceneWindowGeometry(MSceneWindow *window);
    void setSceneWindowGeometries();
    QGraphicsWidget *rootElementForSceneWindow(MSceneWindow *sceneWindow) const;
    void updateSceneWindowRootElement(MSceneWindow *sceneWindow);
    void notifyWidgetsAboutOrientationChange();
    void rotateToAngle(M::OrientationAngle newAngle);
    void setOrientationAngleWithoutAnimation(M::OrientationAngle newAngle);

    void addSceneWindow(MSceneWindow *sceneWindow);
    void removeSceneWindow(MSceneWindow *sceneWindow);
    void addUnmanagedSceneWindow(MSceneWindow *sceneWindow);

    bool isOnDisplay();
    bool canHaveAnimatedTransitions();
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
                                  MPageSwitchAnimation::TransitionDirection direction);

    void createOrientationAnimation();
    void initRelocator();

    MAbstractWidgetAnimation *createAnimationFromSceneWindowType(MSceneWindow* sceneWindow);
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

    void sendCancelEvent(MSceneWindow *window);

    void fastForwardSceneWindowTransitionAnimation(MSceneWindow *sceneWindow);

    MSceneManagerStyleContainer &style();
    const MSceneManagerStyleContainer &style() const;
    virtual MSceneManagerStyleContainer *createStyleContainer() const;

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
    void _q_updateOnDisplayVisibility();

    void _q_unfreezeUI(QAbstractAnimation::State newState,
                       QAbstractAnimation::State oldState);

    void _q_applySceneWindowTransitionsQueuedDueToOrientationAnimation();

    void _q_triggerAsyncPendingOrientationChange();

    void _q_applyPendingOrientationChange();

    void _q_updateRootElementsPositions();
    void _q_setOrientationAngleForWindow();

#ifdef Q_WS_X11
    void _q_updateDecoratorButtonsProperty();
    void updateStatusBarGeometryProperty();
#endif

    void updatePagesAndSheetsGeometry();
    void addPagesAndSheetsGeometryUpdateAnimation(QParallelAnimationGroup* mainAnimation);

    void updateVisibilityOfSceneWindowsBehind(MSceneWindow *referenceSceneWindow, bool newVisibility);
    MSceneWindow *findTopMostSheet();

public:

    MScene *scene;

    QGraphicsWidget *rootElement;
    QGraphicsWidget *homeButtonRootElement;
    QGraphicsWidget *topNavigationBarRootElement;
    QGraphicsWidget *topDockWidgetRootElement;
    QGraphicsWidget *topBorderDecorationRootElement;
    QList<QGraphicsWidget*> rootElementsDisplacedByStatusBar;

    MOrientationAnimation *orientationAnimation;
    MPageSwitchAnimation *pageSwitchAnimation;
    MContentFadeAndSlideAnimation* navigationBarAnimation;

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

    MSceneManager *q_ptr;

    MSceneEventEater *eventEater;

    MSceneManagerStyleContainer *styleContainer;

#ifdef Q_WS_X11
    bool statusBarGeometryPropertyWasSet;
#endif

    MSceneWindowStack sceneWindowStack;
};
#endif
