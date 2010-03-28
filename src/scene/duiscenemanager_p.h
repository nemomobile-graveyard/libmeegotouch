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

#ifndef DUISCENEMANAGER_P_H
#define DUISCENEMANAGER_P_H

#include <QList>
#include <QPointer>

#include "duinamespace.h"
#include "duiscenewindow.h"
#include <duiscenewindowevent_p.h>
#include "duipageswitchanimation.h"

class DuiScene;
class DuiSceneWindow;
class DuiSceneLayerEffect;
class DuiSceneManager;
class DuiNavigationBar;
class DuiBasicOrientationAnimation;
class DuiPageSwitchAnimation;
class DuiWindow;

class QAbstractAnimation;
class QGraphicsWidget;
class QTimeLine;
class QPointF;

class DuiSceneManagerPrivate
{
    Q_DECLARE_PUBLIC(DuiSceneManager)

public:

    void init(DuiScene *scene);
    virtual ~DuiSceneManagerPrivate();

    enum WindowTypeZ {
        PlainSceneWindow = 0,
        ApplicationPage = PlainSceneWindow,          // DuiApplicationPage
        ApplicationMenu = 500,                    // DuiApplicationMenu
        NavigationBar = 1000,                        // DuiNavigationBar
        DockWidget = NavigationBar,                  // DuiDockWidget
        EscapeButtonPanel = 2000,                    // DuiEscapeButtonPanel
        Dialog = 3000,                               // DuiDialog
        MessageBox = Dialog,                         // DuiMessageBox
        ObjectMenu = Dialog,
        ModalSceneWindow = Dialog,                   // DuiModalSceneWindow
        PopupList = 4000,                            // DuiPopupList
        NotificationInformation = 5000,              // DuiNotification - Information
        NotificationEvent = NotificationInformation, // DuiNotification - Event
        Overlay = 6000,                              // DuiOverlay
        Completer = Overlay,                         // DuiCompleter
        HomeButtonPanel = 7000,                      // DuiHomeButtonPanel
        StatusBar = 8000                             // DuiStatusBar
    };

    enum InputPanelPlacement {
        North,
        South,
        Invalid
    };

    int zForWindowType(DuiSceneWindow::WindowType windowType);
    DuiSceneLayerEffect *createLayerEffectForWindow(DuiSceneWindow *window);

    Dui::Orientation orientation(Dui::OrientationAngle angle) const;

    QPointF calculateSceneWindowPosition(DuiSceneWindow *window);
    void setSceneWindowGeometry(DuiSceneWindow *window);
    void setSceneWindowGeometries();
    void notifyWidgetsAboutOrientationChange();
    void rotateToAngle(Dui::OrientationAngle newAngle);
    void setOrientationAngleWithoutAnimation(Dui::OrientationAngle newAngle);

    void attachWindow(DuiSceneWindow *window);
    void detachWindow(DuiSceneWindow *window);

    bool onApplicationPage(QGraphicsItem *item);
    DuiSceneWindow *parentSceneWindow(QGraphicsItem *item);
    InputPanelPlacement inputPanelPlacement(const QRect &inputPanelRect);
    void moveSceneWindow(DuiSceneWindow *window, const QPoint &translation);

    bool validateSceneWindowPreAppearanceStatus(DuiSceneWindow *sceneWindow);

    bool isOnDisplay();
    void produceMustBeResolvedDisplayEvent(DuiSceneWindow *sceneWindow);
    void produceFullyOffDisplayEvents(QGraphicsItem *item);
    void produceSceneWindowEvent(QEvent::Type type, DuiSceneWindow *sceneWindow,
                                 bool animatedTransition);

    void freezeUIForAnimationDuration(QAbstractAnimation *animation);

    void pushPage(DuiSceneWindow *page, bool animatedTransition);
    void popPage(bool animatedTransition);

    void prepareWindowShow(DuiSceneWindow *window);

    void appearSceneWindow(DuiSceneWindow *window,
                      DuiSceneWindow::DeletionPolicy policy,
                      bool animatedTransition);

    void prepareWindowHide(DuiSceneWindow *window);

    void disappearSceneWindow(DuiSceneWindow *window,
                         bool animatedTransition);

    void dismissSceneWindow(DuiSceneWindow *window,
                       bool animatedTransition);

    void startPageSwitchAnimation(DuiSceneWindow *newPage,
                                  DuiSceneWindow *oldPage,
                                  DuiPageSwitchAnimation::PageTransitionDirection direction);

    QRectF calculateAvailableSceneRect(DuiSceneWindow *window);

    void _q_setSenderGeometry();
    void _q_changeGlobalOrientation();
    void _q_emitOrientationChangeFinished();
    void _q_pageShowAnimationFinished();
    void _q_windowShowAnimationFinished();
    void _q_windowHideAnimationFinished();
    void _q_restoreSceneWindow();
    void _q_relocateWindowByInputPanel(const QRect &inputPanelRect);

    //! Prepares current window for SIP.
    void _q_inputPanelOpened();

    //! Restores the current window state after SIP is closed.
    void _q_inputPanelClosed();

    void _q_unFreezeUI();

    void _q_applyQueuedSceneWindowTransitions();

public:

    DuiScene *scene;

    QGraphicsWidget *rootElement;
    DuiBasicOrientationAnimation *orientationAnimation;
    DuiPageSwitchAnimation *pageSwitchAnimation;

    QList<DuiSceneWindow *> *windows;
    QList< QPointer<DuiSceneWindow> > pageHistory;

    Dui::OrientationAngle angle;
    Dui::OrientationAngle newAngle;

    DuiNavigationBar *navBar;
    DuiSceneWindow *escapeButtonPanel;
    QPointer<DuiSceneWindow> currentPage;
    bool navBarHidden;
    bool escapeButtonHidden;

    QGraphicsItem *focusedInputWidget;
    DuiSceneWindow *alteredSceneWindow;
    QPoint sceneWindowTranslation;
    static const int KeyboardSpacing;

    bool pendingSIPClose;

    QPointer<DuiSceneWindow> statusBar;

    class SceneWindowTransition {
        public:
        DuiSceneWindow *sceneWindow;

        enum TransitionType {
            AppearTransition,
            DisappearTransition,
            DismissTransition
        } type;

        // Makes sense only for AppearTransition types.
        DuiSceneWindow::DeletionPolicy policy;

        bool animated;
    };
    QList<SceneWindowTransition> queuedTransitions;

    QPointer<QObject> debugInterface;

    DuiSceneManager *q_ptr;
};

#endif
