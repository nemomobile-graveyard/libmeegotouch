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

#include "mscenemanager.h"
#include "mscenemanager_p.h"

#include <MDebug>
#include <QMouseEvent>
#include <QTimeLine>
#include <QList>
#include <QGraphicsWidget>
#include <QEventLoop>
#include <QInputContext>
#include <QTimer>

#include "mcancelevent.h"
#include "mondisplaychangeevent.h"
#include "morientationchangeevent.h"
#include "mscenewindow.h"
#include "mscenewindow_p.h"
#include "mscenewindowscroller.h"
#include "mdeviceprofile.h"
#include "mdialog.h"
#include "mscene.h"
#include "mscene_p.h"
#include "mscenelayereffect.h"
#include "mnavigationbar.h"
#include "mdockwidget.h"
#include "mobjectmenu.h"
#include "mescapebuttonpanel.h"
#include "mapplication.h"
#include "mwindow.h"
#include "mwindow_p.h"
#include "mapplicationwindow.h"
#include "mapplicationwindow_p.h"
#include "mpannableviewport.h"
#include "mpannableviewportscroller.h"
#include "mtextedit.h"

#include "mscrollchain.h"
#include "minputwidgetrelocator.h"
#include "minputmethodstate.h"

#include "mapplicationpage.h"
#include "morientationtracker.h"

#include "mbasicorientationanimation.h"
#include "mcrossfadedorientationanimation.h"
#include "mabstractwidgetanimation.h"
#include "mpageswitchanimation.h"
#include "mcontentfadeandslideanimation.h"
#include "msceneeventeater.h"

#include <mwidgetslideanimation.h>
#include <mwidgetfadeanimation.h>
#include <mwidgetzoomanimation.h>
#include <mwidgetmoveanimation.h>
#include <mdialoganimation.h>
#include <mwidgetscalefadeanimation.h>
#include <mtheme.h>
#include <QPropertyAnimation>

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
#endif

namespace
{
    //! This is a guess for the SIP close timeout since there is no general way of getting it.
    const int SoftwareInputPanelHideTimer = 500;

    void setParentItemWithoutIncorrectRefocusing(QGraphicsItem *item, QGraphicsItem *parent)
    {
        if (item->parentItem() == parent)
            return;

        // FIXME: Workaround for bug NB#186278.
        QGraphicsItem *focused = item->focusItem();
        if (focused && !focused->hasFocus()) {
            focused->setFlag(QGraphicsItem::ItemIsFocusable, false);
            item->setParentItem(parent);
            focused->setFlag(QGraphicsItem::ItemIsFocusable, true);
        } else {
            // This alone should suffice.
            item->setParentItem(parent);
        }
    }
}

void MSceneWindowStack::add(MSceneWindow *newSceneWindow)
{
    Q_ASSERT(!m_list.contains(newSceneWindow));

    if (m_list.isEmpty()) {
        m_list.append(newSceneWindow);
        return;
    }

    MSceneWindow *sceneWindow;
    int i = m_list.count() - 1;
    bool inserted = false;
    do {
        sceneWindow = m_list.at(i);
        if (sceneWindow->zValue() <= newSceneWindow->zValue()) {
            // put newSceneWindow in front of sceneWindow
            m_list.insert(i + 1, newSceneWindow);
            inserted = true;
        } else if (i == 0) {
            m_list.prepend(newSceneWindow);
            inserted = true;
        } else {
            --i;
        }
    } while(!inserted);
}

void MSceneWindowStack::remove(MSceneWindow *sceneWindow)
{
    m_list.removeAll(sceneWindow);
}

MSceneManagerPrivate::MSceneManagerPrivate()
    :scene(0)
    ,rootElement(0)
    ,homeButtonRootElement(0)
    ,topNavigationBarRootElement(0)
    ,topDockWidgetRootElement(0)
    ,topBorderDecorationRootElement(0)
    ,orientationAnimation(0)
    ,pageSwitchAnimation(0)
    ,navigationBarAnimation(0)
    ,angle(M::Angle0)
    ,newAngle(M::Angle0)
    ,pendingRotation(0)
    ,q_ptr(0)
    ,eventEater(0)
    ,styleContainer(0)
#ifdef Q_WS_X11
    ,statusBarGeometryPropertyWasSet(false)
#endif
{
}

void MSceneManagerPrivate::init(MScene *scene)
{
    Q_Q(MSceneManager);

    this->scene = scene;

    initOrientationAngles();

    rootElement = new QGraphicsWidget();

    homeButtonRootElement = new QGraphicsWidget(rootElement);
    homeButtonRootElement->setZValue(zForWindowType(MSceneWindow::HomeButtonPanel));
    homeButtonRootElement->setFlag(QGraphicsItem::ItemHasNoContents);
    rootElementsDisplacedByStatusBar << homeButtonRootElement;

    topNavigationBarRootElement = new QGraphicsWidget(rootElement);
    topNavigationBarRootElement->setZValue(zForWindowType(MSceneWindow::NavigationBar));
    topNavigationBarRootElement->setFlag(QGraphicsItem::ItemHasNoContents);
    rootElementsDisplacedByStatusBar << topNavigationBarRootElement;

    topDockWidgetRootElement = new QGraphicsWidget(rootElement);
    topDockWidgetRootElement->setZValue(zForWindowType(MSceneWindow::DockWidget));
    topDockWidgetRootElement->setFlag(QGraphicsItem::ItemHasNoContents);
    rootElementsDisplacedByStatusBar << topDockWidgetRootElement;

    topBorderDecorationRootElement = new QGraphicsWidget(rootElement);
    topBorderDecorationRootElement->setZValue(zForWindowType(MSceneWindow::BorderDecoration));
    topBorderDecorationRootElement->setFlag(QGraphicsItem::ItemHasNoContents);
    rootElementsDisplacedByStatusBar << topBorderDecorationRootElement;

    rootElement->setTransformOriginPoint(QPointF(q->visibleSceneSize().width() / 2.f, q->visibleSceneSize().height() / 2.f));
    scene->addItem(rootElement);

    eventEater = new MSceneEventEater();
    scene->addItem(eventEater);
    QSize eventEaterSize = q->visibleSceneSize(MDeviceProfile::instance()->orientationFromAngle(M::Angle0));
    eventEater->setGeometry(QRectF(QPointF(0.0, 0.0), eventEaterSize));
    createOrientationAnimation();

    initRelocator();

    pageSwitchAnimation = qobject_cast<MPageSwitchAnimation*>(MTheme::animation(style()->pageSwitchAnimation()));
    q->connect(pageSwitchAnimation, SIGNAL(finished()),
            SLOT(_q_onPageSwitchAnimationFinished()));

    if (pageSwitchAnimation) {
        navigationBarAnimation = new MContentFadeAndSlideAnimation(q);
        pageSwitchAnimation->addAnimation(navigationBarAnimation);
    }

    setOrientationAngleWithoutAnimation(newAngle);

    q->connect(MTheme::instance(), SIGNAL(themeChangeCompleted()), q, SLOT(_q_updateRootElementsPositions()));
}

void MSceneManagerPrivate::initOrientationAngles()
{
    // set the angle to that of the topmost window, if one exists
    MWindow *activeWindow = MApplication::activeWindow();

    MWindow *viewingWindow = 0;
    if (scene->views().size() > 0) {
        viewingWindow = qobject_cast<MWindow *>(scene->views().at(0));
        Q_ASSERT(viewingWindow);
    }

    if (activeWindow && activeWindow != viewingWindow)
        angle = activeWindow->orientationAngle();
    else
        angle = MOrientationTracker::instance()->orientationAngle();
    newAngle = angle;
}

void MSceneManagerPrivate::createOrientationAnimation()
{
    Q_Q(MSceneManager);

    QRectF visibleSceneRect = QRectF(QPointF(0.0, 0.0),
        q->visibleSceneSize(MDeviceProfile::instance()->orientationFromAngle(M::Angle0)));

    //TODO: get this from theme
    orientationAnimation = new MCrossFadedOrientationAnimation(visibleSceneRect);
    //orientationAnimation = new MBasicOrientationAnimation(q->visibleSceneSize(M::Landscape));

    orientationAnimation->setRootElement(rootElement);

    q->connect(orientationAnimation, SIGNAL(orientationChanged()), SLOT(_q_changeGlobalOrientationAngle()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_emitOrientationChangeFinished()));
    q->connect(orientationAnimation, SIGNAL(finished()),
            SLOT(_q_applySceneWindowTransitionsQueuedDueToOrientationAnimation()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_triggerAsyncPendingOrientationChange()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_updateOnDisplayVisibility()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_setOrientationAngleForWindow()));
}

void MSceneManagerPrivate::initRelocator()
{
    Q_Q(MSceneManager);

    inputWidgetRelocator = QPointer<MInputWidgetRelocator>(
        new MInputWidgetRelocator(*q, rootElement,
                                  MDeviceProfile::instance()->orientationFromAngle(newAngle)));

    // Register scene window scroller.
    QSharedPointer<MSceneWindowScroller> sceneWindowScroller(new MSceneWindowScroller);
    MScrollChain::registerScroller<MSceneWindow>(sceneWindowScroller);

    // Connect signals for scene window displacement.
    q->connect(sceneWindowScroller.data(), SIGNAL(sceneWindowDislocationRequest(MSceneWindow *, QPointF)),
               q, SLOT(_q_dislocateSceneWindow(MSceneWindow *, QPointF)), Qt::DirectConnection);
    q->connect(sceneWindowScroller.data(), SIGNAL(sceneWindowUndoDislocationRequest(MSceneWindow *)),
               q, SLOT(_q_undoSceneWindowDislocation(MSceneWindow *)), Qt::DirectConnection);

    // Register pannable viewport scroller.
    QSharedPointer<MPannableViewportScroller> pannableViewportScroller(new MPannableViewportScroller);
    MScrollChain::registerScroller<MPannableViewport>(pannableViewportScroller);
}

MSceneManagerPrivate::~MSceneManagerPrivate()
{
    delete inputWidgetRelocator;
    delete orientationAnimation;
    delete pageSwitchAnimation;
    delete pendingRotation;
    delete styleContainer;
}

int MSceneManagerPrivate::zForWindowType(MSceneWindow::WindowType windowType)
{
    int z = 0;
    switch (windowType) {
    case MSceneWindow::PlainSceneWindow:
        z = MSceneManagerPrivate::PlainSceneWindow;
        break;
    case MSceneWindow::ApplicationPage:
        z = MSceneManagerPrivate::ApplicationPage;
        break;
    case MSceneWindow::NavigationBar:
        z = MSceneManagerPrivate::NavigationBar;
        break;
    case MSceneWindow::DockWidget:
        z = MSceneManagerPrivate::DockWidget;
        break;
    case MSceneWindow::EscapeButtonPanel:
        z = MSceneManagerPrivate::EscapeButtonPanel;
        break;
    case MSceneWindow::Dialog:
        z = MSceneManagerPrivate::Dialog;
        break;
    case MSceneWindow::ModalSceneWindow:
        z = MSceneManagerPrivate::ModalSceneWindow;
        break;
    case MSceneWindow::MessageBox:
        z = MSceneManagerPrivate::MessageBox;
        break;
    case MSceneWindow::ApplicationMenu:
        z = MSceneManagerPrivate::ApplicationMenu;
        break;
    case MSceneWindow::PopupList:
        z = MSceneManagerPrivate::PopupList;
        break;
    case MSceneWindow::NotificationInformation:
        z = MSceneManagerPrivate::NotificationInformation;
        break;
    case MSceneWindow::NotificationEvent:
        z = MSceneManagerPrivate::NotificationEvent;
        break;
    case MSceneWindow::Overlay:
        z = MSceneManagerPrivate::Overlay;
        break;
    case MSceneWindow::ObjectMenu:
        z = MSceneManagerPrivate::ObjectMenu;
        break;
    case MSceneWindow::Completer:
        z = MSceneManagerPrivate::Completer;
        break;
    case MSceneWindow::HomeButtonPanel:
        z = MSceneManagerPrivate::HomeButtonPanel;
        break;
    case MSceneWindow::StatusBar:
        z = MSceneManagerPrivate::StatusBar;
        break;
    case MSceneWindow::Sheet:
        z = MSceneManagerPrivate::Sheet;
        break;
    case MSceneWindow::BorderDecoration:
        z = MSceneManagerPrivate::BorderDecoration;
        break;
    default:
        z = 0;
        // Should not get here. Only scene layer effect does not have it's
        // case statement and z is not asked for effect layer
        break;
    }
    return z;
}

void MSceneManagerPrivate::_q_setSenderGeometry()
{
    Q_Q(MSceneManager);

    MSceneWindow *window = qobject_cast<MSceneWindow *>(q->sender());
    if (window)
        setSceneWindowGeometry(window);
}

void MSceneManagerPrivate::_q_changeGlobalOrientationAngle()
{
    Q_Q(MSceneManager);

    M::OrientationAngle oldAngle = angle;
    M::Orientation oldOrientation = MDeviceProfile::instance()->orientationFromAngle(angle);
    M::Orientation newOrientation = MDeviceProfile::instance()->orientationFromAngle(newAngle);
    angle = newAngle;

    // Needs to be called on every angle change and before scene window geometries are changed.
    inputWidgetRelocator->handleRotationBegin(newOrientation);

    if (oldOrientation != newOrientation) {
        emit q->orientationAboutToChange(q->orientation());

        notifyWidgetsAboutOrientationChange();
        setSceneWindowGeometries();

        // emit signal after sending the orientation event to widgets (in case someone
        // would like to connect to the signal and get correct size hints for widgets)
        emit q->orientationChanged(newOrientation);
    }
    if (oldAngle != angle)
        emit q->orientationAngleChanged(angle);
}

void MSceneManagerPrivate::_q_emitOrientationChangeFinished()
{
    Q_Q(MSceneManager);

    emit q->orientationChangeFinished(q->orientation());
#ifdef Q_WS_X11
    _q_updateDecoratorButtonsProperty();
    updateStatusBarGeometryProperty();
#endif

    // Needs to be called on every angle change and after animation and scene window
    // geometries have been changed.
    inputWidgetRelocator->handleRotationFinished();
}

void MSceneManagerPrivate::_q_unfreezeUI(QAbstractAnimation::State newState,
                                         QAbstractAnimation::State oldState)
{
    Q_Q(MSceneManager);
    Q_UNUSED(oldState);

    if (newState == QAbstractAnimation::Running)
        return;

    eventEater->hide();

    QAbstractAnimation *animation = dynamic_cast<QAbstractAnimation *>(q->sender());
    if (animation == 0)
        return;

    QObject::disconnect(animation,
                        SIGNAL(stateChanged(QAbstractAnimation::State,
                                            QAbstractAnimation::State)),
                        q, SLOT(_q_unfreezeUI(QAbstractAnimation::State,
                                              QAbstractAnimation::State)));
}

void MSceneManagerPrivate::_q_applySceneWindowTransitionsQueuedDueToOrientationAnimation()
{
    MSceneWindowTransition transition;

    while (queuedTransitionsOrientationAnimation.count() > 0) {
        transition = queuedTransitionsOrientationAnimation.takeFirst();

        applySceneWindowTransition(&transition);
    }
}

void MSceneManagerPrivate::applySceneWindowTransition(MSceneWindowTransition *transition)
{
    switch (transition->type) {
        case MSceneWindowTransition::AppearTransition:
            appearSceneWindow(transition->sceneWindow, transition->policy, transition->animated);
            break;
        case MSceneWindowTransition::DisappearTransition:
            disappearSceneWindow(transition->sceneWindow, transition->animated);
            break;
        default:
            // Should never occur.
            qFatal("MSceneManager: Invalid MSceneWindowTransition::TransitionType value.");
    }
}

void MSceneManagerPrivate::applySceneWindowTransitionsFromPageSwitchQueue()
{
    bool done = queuedTransitionsPageSwitchAnimation.isEmpty();
    MSceneWindowTransition transition;

    while (!done) {
        transition = queuedTransitionsPageSwitchAnimation.takeFirst();

        applySceneWindowTransition(&transition);

        // Since an application page will cause another page switch animation,
        // we will have to stop processing the queue until this new page switch
        // animation finishes.
        if (transition.sceneWindow->windowType() == MSceneWindow::ApplicationPage ||
                queuedTransitionsPageSwitchAnimation.isEmpty()) {
            done = true;
        }
    }
}

void MSceneManagerPrivate::removeSceneWindowFromTransitionQueue(MSceneWindow *sceneWindow,
        QList<MSceneWindowTransition> &transitionQueue)
{
    MSceneWindowTransition transition;
    int i;

    i = 0;
    while (i < transitionQueue.count()) {
        transition = transitionQueue[i];
        if (transition.sceneWindow == sceneWindow) {
            transitionQueue.removeAt(i);
        } else {
            ++i;
        }
    }
}

void MSceneManagerPrivate::_q_updateOnDisplayVisibility()
{
    foreach(MSceneWindow* sceneWindow, windows) {
        if (sceneWindow && sceneWindow->sceneWindowState() != MSceneWindow::Disappeared)
            produceMustBeResolvedDisplayEvent(sceneWindow);
    }
}

void MSceneManagerPrivate::_q_triggerAsyncPendingOrientationChange()
{
    Q_Q(MSceneManager);

    if (pendingRotation != 0) {
        // This slot is called in the context of the finished() signal.
        // Applying the orientation change must be done asynchronously, so that
        // other slots connected to finished() are called before.
        QTimer::singleShot(0, q, SLOT(_q_applyPendingOrientationChange()));
    }
}

void MSceneManagerPrivate::_q_applyPendingOrientationChange()
{
    Q_Q(MSceneManager);

    if (pendingRotation != 0) {
        const M::OrientationAngle pendingAngle = pendingRotation->angle;
        const MSceneManager::TransitionMode pendingMode = pendingRotation->mode;

        delete pendingRotation;
        pendingRotation = 0;

        q->setOrientationAngle(pendingAngle, pendingMode);
    }
}

void MSceneManagerPrivate::_q_onSceneWindowAppearanceAnimationFinished()
{
    Q_Q(MSceneManager);

    QAbstractAnimation *appearanceAnimation = qobject_cast<QAbstractAnimation *>(q->sender());
    Q_ASSERT(appearanceAnimation);

    QObject::disconnect(appearanceAnimation, SIGNAL(finished()),
            q, SLOT(_q_onSceneWindowAppearanceAnimationFinished()));

    MSceneWindow *sceneWindow = qobject_cast<MSceneWindow *>(appearanceAnimation->parent());
    Q_ASSERT(sceneWindow);

    setSceneWindowState(sceneWindow, MSceneWindow::Appeared);

    if (sceneWindow->d_func()->queuedTransition) {
        MSceneWindowTransition *transition = sceneWindow->d_func()->queuedTransition;
        sceneWindow->d_func()->queuedTransition = 0;
        applySceneWindowTransition(transition);
        delete transition;
    }
}

void MSceneManagerPrivate::_q_onSceneWindowDisappearanceAnimationFinished()
{
    Q_Q(MSceneManager);

    QAbstractAnimation *disappearanceAnimation = qobject_cast<QAbstractAnimation *>(q->sender());
    Q_ASSERT(disappearanceAnimation);

    QObject::disconnect(disappearanceAnimation, SIGNAL(finished()),
            q, SLOT(_q_onSceneWindowDisappearanceAnimationFinished()));

    MSceneWindow  *sceneWindow = qobject_cast<MSceneWindow *>(disappearanceAnimation->parent());
    Q_ASSERT(sceneWindow);

    bool sceneWindowWillBeDeleted = (sceneWindow->deletionPolicy() == MSceneWindow::DestroyWhenDone) ||
                ((sceneWindow->deletionPolicy() == MSceneWindow::DestroyWhenDismissed)
                 && sceneWindow->d_func()->dismissed);

    setSceneWindowState(sceneWindow, MSceneWindow::Disappeared);

    if (!sceneWindowWillBeDeleted && sceneWindow->d_func()->queuedTransition) {
        MSceneWindowTransition *transition = sceneWindow->d_func()->queuedTransition;
        sceneWindow->d_func()->queuedTransition = 0;
        applySceneWindowTransition(transition);
        delete transition;
    }
}

void MSceneManagerPrivate::_q_onPageSwitchAnimationFinished()
{
    MSceneWindow *oldPage = pageSwitchAnimation->oldPage();
    MSceneWindow *newPage = pageSwitchAnimation->newPage();

    if (oldPage)
        setSceneWindowState(oldPage, MSceneWindow::Disappeared);

    if (newPage)
        setSceneWindowState(newPage, MSceneWindow::Appeared);

    applySceneWindowTransitionsFromPageSwitchQueue();
}

void MSceneManagerPrivate::_q_dislocateSceneWindow(MSceneWindow *sceneWindow,
                                                   const QPointF &displacement)
{
    if (!windows.contains(sceneWindow)) {
        return;
    }

    if (sceneWindow->windowType() == MSceneWindow::LayerEffect) {
        // Not supported.
        return;
    }

    // let's use an alias
    QGraphicsItem *&displacementItem = sceneWindow->d_func()->displacementItem;

    if (!displacementItem) {
        displacementItem = new QGraphicsWidget;
        displacementItem->setFlag(QGraphicsItem::ItemHasNoContents, true);

        setParentItemWithoutIncorrectRefocusing(displacementItem, sceneWindow->parentItem());
        setParentItemWithoutIncorrectRefocusing(sceneWindow, displacementItem);

        // Don't change the z value.
        displacementItem->setZValue(sceneWindow->zValue());
    }

    displacementItem->setPos(displacementItem->pos() + displacement);
}

void MSceneManagerPrivate::_q_undoSceneWindowDislocation(MSceneWindow *sceneWindow)
{
    if (!windows.contains(sceneWindow)) {
        return;
    }

    // let's use an alias
    QGraphicsItem *&displacementItem = sceneWindow->d_func()->displacementItem;

    if (displacementItem) {
        displacementItem->setPos(0.0, 0.0);

        setParentItemWithoutIncorrectRefocusing(sceneWindow, displacementItem->parentItem());

        delete displacementItem;
        displacementItem = 0;
    }
}

void MSceneManager::ensureCursorVisible()
{
    Q_D(MSceneManager);
    d->inputWidgetRelocator->update();
}

void MSceneManagerPrivate::addUnmanagedSceneWindow(MSceneWindow *sceneWindow)
{
    Q_Q(MSceneManager);
    Q_ASSERT(!windows.contains(sceneWindow));

    windows.append(sceneWindow);
    sceneWindow->d_func()->sceneManager = q;

    // window could have been added to another scene manually beforehand
    // remove it in that case, to avoid Qt's assert
    if (sceneWindow->scene() && sceneWindow->scene() != scene)
        sceneWindow->scene()->removeItem(sceneWindow);

    // add scene window to the scene
    // Now its sceneManager() method will return the correct result.
    // It will also transfer the ownership of the scene window to the scene.

    setParentItemWithoutIncorrectRefocusing(sceneWindow, rootElementForSceneWindow(sceneWindow));

    sceneWindow->setZValue(zForWindowType(sceneWindow->windowType()));

    sceneWindow->hide();

    if (sceneWindow->windowType() == MSceneWindow::NavigationBar && navigationBarAnimation)
        navigationBarAnimation->setTargetWidget(sceneWindow);
}

void MSceneManagerPrivate::addSceneWindow(MSceneWindow *sceneWindow)
{
    Q_Q(MSceneManager);

    if (!sceneWindow->d_func()->sceneManager) {
        // Just add it
        addUnmanagedSceneWindow(sceneWindow);
        // This window was not receiving any events. Might not know about orientation change yet.
        MOrientationChangeEvent event(MDeviceProfile::instance()->orientationFromAngle(angle));
        scene->sendEvent(sceneWindow, &event);
    } else {
        if (sceneWindow->d_func()->sceneManager != q) {
            // scene window belongs to another scene manager, remove it from him.
            sceneWindow->d_func()->sceneManager->d_func()->removeSceneWindow(sceneWindow);

            addUnmanagedSceneWindow(sceneWindow);
        } else {
            // scene window already belongs to this scene manager.
            // Thus nothing to do.
            Q_ASSERT(windows.contains(sceneWindow));
        }
    }
}

void MSceneManagerPrivate::removeSceneWindow(MSceneWindow *sceneWindow)
{
    Q_Q(MSceneManager);

    if (sceneWindow->d_func()->sceneManager != q) {
        // It's not being managed by this scene manager.
        return;
    }

    Q_ASSERT(windows.contains(sceneWindow));

    if (sceneWindow->windowType() == MSceneWindow::ApplicationPage) {
        if (currentPage == sceneWindow)
            popPage(false);
        else
            pageHistory.removeOne(sceneWindow);
    }

    sceneWindow->d_func()->policy = MSceneWindow::KeepWhenDone;
    setSceneWindowState(sceneWindow, MSceneWindow::Disappeared);

    if (sceneWindow->d_func()->queuedTransition) {
        delete sceneWindow->d_func()->queuedTransition;
        sceneWindow->d_func()->queuedTransition = 0;
    }

    removeSceneWindowFromTransitionQueue(sceneWindow, queuedTransitionsOrientationAnimation);
    removeSceneWindowFromTransitionQueue(sceneWindow, queuedTransitionsPageSwitchAnimation);

    windows.removeOne(sceneWindow);
    sceneWindow->d_func()->sceneManager = 0;

    _q_undoSceneWindowDislocation(sceneWindow);

    // Give ownership of scene window to the caller.
    scene->removeItem(sceneWindow);
}

void MSceneManagerPrivate::setupLayerEffectForSceneWindow(MSceneWindow *window)
{
    MSceneLayerEffect *effect = 0;

    switch (window->windowType()) {
    case MSceneWindow::Dialog:
    case MSceneWindow::MessageBox:
    case MSceneWindow::ModalSceneWindow:
    case MSceneWindow::ApplicationMenu:
        effect = new MSceneLayerEffect("messagebox");
        break;
    case MSceneWindow::PopupList:
    case MSceneWindow::ObjectMenu:
        effect = new MSceneLayerEffect("objectmenu");
        break;
    default:
        return;
    }

    //resize the effect layer
    setSceneWindowGeometry(effect);

    QGraphicsItem *sceneWindowAndLayerEffectBinder = new QGraphicsWidget;
    sceneWindowAndLayerEffectBinder->setFlag(QGraphicsItem::ItemHasNoContents);

    // Add effect to scene via rootElement
    QGraphicsItem *binderParent = window->parentItem() ?
                                  window->parentItem() : rootElementForSceneWindow(window);

    setParentItemWithoutIncorrectRefocusing(sceneWindowAndLayerEffectBinder, binderParent);
    sceneWindowAndLayerEffectBinder->setZValue(zForWindowType(window->windowType()));

    // Add window as child of the binder
    setParentItemWithoutIncorrectRefocusing(window, sceneWindowAndLayerEffectBinder);
    window->d_func()->effect = effect;

    effect->setParentItem(sceneWindowAndLayerEffectBinder);

    // Make sure the effect layer is below its corresponding scene window
    effect->setZValue(0);
    window->setZValue(1);
}

void MSceneManagerPrivate::destroyLayerEffectForSceneWindow(MSceneWindow *sceneWindow)
{
    MSceneLayerEffect *&effect = sceneWindow->d_func()->effect;
    if (effect) {
        QGraphicsItem *sceneWindowAndLayerEffectBinder = sceneWindow->parentItem();
        Q_ASSERT(effect->parentItem() == sceneWindowAndLayerEffectBinder);

        setParentItemWithoutIncorrectRefocusing(sceneWindow,
                                                sceneWindowAndLayerEffectBinder->parentItem());


        scene->removeItem(sceneWindowAndLayerEffectBinder);
        delete sceneWindowAndLayerEffectBinder; // will bring effect along with it.
        effect = 0;
    }
}

QGraphicsWidget *MSceneManagerPrivate::rootElementForSceneWindow(MSceneWindow *sceneWindow) const
{
    QGraphicsWidget *root = 0;
    switch (sceneWindow->windowType()) {
        case MSceneWindow::EscapeButtonPanel:
        case MSceneWindow::NavigationBar:
        case MSceneWindow::ApplicationMenu:
            if (sceneWindow->alignment().testFlag(Qt::AlignTop)) {
                root = topNavigationBarRootElement;
            } else {
                root = rootElement;
            }
            break;
        case MSceneWindow::HomeButtonPanel:
        case MSceneWindow::NotificationInformation:
        case MSceneWindow::NotificationEvent:
            root = homeButtonRootElement;
            break;
        case MSceneWindow::DockWidget:
            if (sceneWindow->alignment().testFlag(Qt::AlignTop)) {
                root = topDockWidgetRootElement;
            } else {
                root = rootElement;
            }
            break;
        case MSceneWindow::BorderDecoration:
            if (sceneWindow->alignment().testFlag(Qt::AlignTop)) {
                root = topBorderDecorationRootElement;
            } else {
                root = rootElement;
            }
            break;
        default:
            root = rootElement;
            break;
    }
    return root;
}

void MSceneManagerPrivate::updateSceneWindowRootElement(MSceneWindow *sceneWindow)
{
    QGraphicsWidget *newRootElement = rootElementForSceneWindow(sceneWindow);
    QGraphicsWidget *currentRootElement;

    if (sceneWindow->d_func()->displacementItem) {
        // skip the displacement item
        currentRootElement = sceneWindow->d_func()->displacementItem->parentWidget();
    } else {
        currentRootElement = sceneWindow->parentWidget();
    }

    if (sceneWindow->d_func()->effect) {
        // it's not yet the root element. we gotta skip the effect binder as well.
        currentRootElement = currentRootElement->parentWidget();
    }

    if (currentRootElement != newRootElement) {
        setParentItemWithoutIncorrectRefocusing(sceneWindow, newRootElement);
    }
}

void MSceneManagerPrivate::setSceneWindowGeometries()
{
    const int size = windows.size();
    MSceneWindow *sceneWindow;

    for (int i = 0; i < size; ++i) {
        sceneWindow = windows.at(i);
        if (sceneWindow->sceneWindowState() != MSceneWindow::Disappeared) {
            _q_undoSceneWindowDislocation(sceneWindow);
            setSceneWindowGeometry(sceneWindow);
        }
    }
}

void MSceneManagerPrivate::setSceneWindowGeometry(MSceneWindow *window)
{
    if (window->isManagedManually())
        return;

    // A change in alignment may cause a change in the rootElement where the
    // scene window should be placed
    updateSceneWindowRootElement(window);

    QRectF geom = calculateSceneWindowGeometry(window);

    // restart running navigationbar appearance animation to update animation endValues
    MAbstractWidgetAnimation* animation = window->d_func()->appearanceAnimation;
    if (window->windowType() == MSceneWindow::NavigationBar &&
        animation && animation->state() == QAbstractAnimation::Running)
    {
        animation->stop();
        animation->restoreTargetWidgetState();
        window->setGeometry(geom);
        animation->start();
    } else {
        window->setGeometry(geom);
    }
}

#ifdef Q_WS_X11
void MSceneManagerPrivate::_q_updateDecoratorButtonsProperty()
{
    if (scene->views().isEmpty())
        return;

    QRectF homeButtonGeometry;
    QRectF closeButtonGeometry;
    QList<MSceneWindow*> sceneWindowsAboveCloseButton;
    QList<MSceneWindow*> sceneWindowsAboveHomeButton;

    foreach (MSceneWindow *window, windows) {
        if (window->sceneWindowState() != MSceneWindow::Disappeared) {
            if (window->zValue() > MSceneManagerPrivate::HomeButtonPanel)
                sceneWindowsAboveHomeButton << window;
            else if (window->zValue() > MSceneManagerPrivate::EscapeButtonPanel)
                sceneWindowsAboveCloseButton << window;

            switch (window->windowType()) {
            case MSceneWindow::EscapeButtonPanel:
                {
                    MEscapeButtonPanel *escapeButtonPanel = static_cast<MEscapeButtonPanel*>(window);
                    QVariant property = escapeButtonPanel->property("hasCloseButton");
                    bool hasCloseButton = property.isValid() ? property.toBool() : true;

                    if (escapeButtonPanel->escapeMode() == MEscapeButtonPanelModel::CloseMode &&
                        hasCloseButton) {
                        closeButtonGeometry = window->mapRectToScene(
                                QRectF(QPointF(),window->geometry().size()));
                    }
                    break;
                }
            case MSceneWindow::HomeButtonPanel:
                homeButtonGeometry = window->mapRectToScene(QRectF(QPointF(),window->geometry().size()));
                break;
            default:
                break;
            }
        }
    }


    bool homeButtonObscured = false;
    bool closeButtonObscured = false;

    if (homeButtonGeometry.isValid() || closeButtonGeometry.isValid()) {
        foreach (MSceneWindow *window, sceneWindowsAboveHomeButton) {
            closeButtonObscured = windowIntersectsRect(closeButtonGeometry,window);
            if (windowIntersectsRect(homeButtonGeometry,window)) {
                homeButtonObscured = true;
                break;
            }
        }

        if (!closeButtonObscured) {
            foreach (MSceneWindow *window, sceneWindowsAboveCloseButton) {
                if (windowIntersectsRect(closeButtonGeometry,window)) {
                    closeButtonObscured = true;
                    break;
                }
            }
        }
    }

    long data[8] = {0};
    if (!homeButtonObscured) {
        data[0] = homeButtonGeometry.x();
        data[1] = homeButtonGeometry.y();
        data[2] = homeButtonGeometry.width();
        data[3] = homeButtonGeometry.height();
    }
    if (!closeButtonObscured) {
        data[4] = closeButtonGeometry.x();
        data[5] = closeButtonGeometry.y();
        data[6] = closeButtonGeometry.width();
        data[7] = closeButtonGeometry.height();
    }

    Display *dpy = QX11Info::display();

    Atom a = XInternAtom(dpy, "_MEEGOTOUCH_DECORATOR_BUTTONS", False);
    foreach(QGraphicsView *view, scene->views()) {
        Window w = view->effectiveWinId();
        XChangeProperty(dpy, w, a, XA_CARDINAL, 32, PropModeReplace,
                        (unsigned char*)data, 8);
    }
}

bool MSceneManagerPrivate::windowIntersectsRect(const QRectF &rect, MSceneWindow *window)
{
    return rect.intersects(window->geometry()) ||
        (window->d_func()->effect && rect.intersects(window->d_func()->effect->geometry()));
}

void MSceneManagerPrivate::updateStatusBarGeometryProperty()
{
    if (!statusBar && !statusBarGeometryPropertyWasSet)
        return;

    unsigned long data[4] = {0};

    if (statusBar) {
        QRectF statusBarGeometry = statusBar->geometry();
        QRectF mappedStatusBarGeometry = statusBar->mapRectToScene(statusBarGeometry);
        data[0] = mappedStatusBarGeometry.x();
        data[1] = mappedStatusBarGeometry.y();
        data[2] = mappedStatusBarGeometry.width();
        data[3] = mappedStatusBarGeometry.height();
    }

    Display *dpy = QX11Info::display();

    Atom a = XInternAtom(dpy, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY", False);
    foreach (QGraphicsView *view, scene->views()) {
        Window w = view->effectiveWinId();
        if (!statusBar)
            XDeleteProperty(dpy, w, a);
        else
            XChangeProperty(dpy, w, a, XA_CARDINAL, 32, PropModeReplace,
                            (unsigned char*)data, 4);
    }

    statusBarGeometryPropertyWasSet = statusBar ? true : false;
}
#endif

void MSceneManagerPrivate::notifyWidgetsAboutOrientationChange()
{
    MScenePrivate::notifySceneAboutOrientationChange(scene, MDeviceProfile::instance()->orientationFromAngle(angle));
}

QRectF MSceneManagerPrivate::calculateAvailableSceneRect(MSceneWindow *sceneWindow) const
{
    Q_Q(const MSceneManager);
    QSizeF sceneSize = q->visibleSceneSize(MDeviceProfile::instance()->orientationFromAngle(angle));
    QRectF availableSceneRect(QPointF(0,0), sceneSize);

    if ((sceneWindow->windowType() == MSceneWindow::Sheet || sceneWindow->windowType() == MSceneWindow::ApplicationPage)
        && statusBar && (statusBar->sceneWindowState() == MSceneWindow::Appearing ||
                         statusBar->sceneWindowState() == MSceneWindow::Appeared))
    {
        availableSceneRect.setTop(statusBar->size().height());
    }

    return availableSceneRect;
}

QRectF MSceneManagerPrivate::calculateSceneWindowGeometry(MSceneWindow *window) const
{
    QRectF availableSceneRect = calculateAvailableSceneRect(window);
    Qt::Alignment alignment = window->alignment();

    //If the preferred size returned from sizeHint is larger than the maximum size, it will get reduced.
    //But this means that we need to get the new height for the new width..
    QSizeF windowSize = window->effectiveSizeHint(Qt::PreferredSize, QSizeF(window->preferredWidth(), -1));

    // scene window shouldn't be bigger than the available scene rect.
    windowSize = windowSize.boundedTo(availableSceneRect.size());

    QPointF pos;

    if (alignment.testFlag(Qt::AlignLeft))
        pos.rx() = availableSceneRect.x();
    else if (alignment.testFlag(Qt::AlignHCenter))
        pos.rx() = availableSceneRect.x() + (availableSceneRect.width() / 2 - windowSize.width() / 2);
    else if (alignment.testFlag(Qt::AlignRight))
        pos.rx() = availableSceneRect.x() + (availableSceneRect.width() - windowSize.width());
    else
        pos.rx() = availableSceneRect.x();

    if (alignment.testFlag(Qt::AlignTop))
        pos.ry() = availableSceneRect.y();
    else if (alignment.testFlag(Qt::AlignVCenter))
        pos.ry() = availableSceneRect.y() + (availableSceneRect.height() / 2 - windowSize.height() / 2);
    else if (alignment.testFlag(Qt::AlignBottom))
        pos.ry() = availableSceneRect.y() + (availableSceneRect.height() - windowSize.height());
    else
        pos.ry() = availableSceneRect.y();

    pos += window->offset();

    return QRectF(pos, windowSize);
}

void MSceneManagerPrivate::rotateToAngle(M::OrientationAngle newAngle)
{
    if (this->newAngle == newAngle)
        return;

    this->newAngle = newAngle;
    orientationAnimation->setTargetRotationAngle(angle, newAngle);
    orientationAnimation->start();
    freezeUIForAnimationDuration(orientationAnimation);
}

void MSceneManagerPrivate::setOrientationAngleWithoutAnimation(M::OrientationAngle newAngle)
{
    this->newAngle = newAngle;
    _q_changeGlobalOrientationAngle();

    QSize nativeScreenSize = MDeviceProfile::instance()->resolution();
    M::Orientation nativeOrientation = MDeviceProfile::instance()->orientationFromAngle(M::Angle0);

    rootElement->setRotation(newAngle);
    if (MDeviceProfile::instance()->orientationFromAngle(newAngle) == nativeOrientation) {
        rootElement->setPos(0, 0);
        rootElement->setTransformOriginPoint(nativeScreenSize.width() / 2,
                                             nativeScreenSize.height() / 2);
    } else {
        rootElement->setPos((nativeScreenSize.width() - nativeScreenSize.height()) / 2,
                            (nativeScreenSize.height() - nativeScreenSize.width()) / 2);
        rootElement->setTransformOriginPoint(nativeScreenSize.height() / 2,
                                             nativeScreenSize.width() / 2);
    }

    _q_emitOrientationChangeFinished();
    _q_updateOnDisplayVisibility();
    _q_setOrientationAngleForWindow();
}

//we need to update MWindow orienentation angle
void MSceneManagerPrivate::_q_setOrientationAngleForWindow()
{
    Q_Q(MSceneManager);

    foreach(QGraphicsView *view, q->scene()->views()) {
        if(!view)
            continue;
        MWindow* win = qobject_cast<MWindow *>(view);
        if(win && win->d_func()->angle !=angle) {
            win->d_func()->angle = angle;
#ifdef Q_WS_X11
            win->d_func()->setX11OrientationAngleProperty(angle);
#endif
        }
    }
}

bool MSceneManagerPrivate::isOnDisplay()
{
    Q_Q(MSceneManager);

    QList<QGraphicsView *> viewsList = q->scene()->views();
    MWindow *window = 0;
    bool result = false;
    int i = 0;

    while (result == false && i < viewsList.count()) {

        window = qobject_cast<MWindow *>(viewsList[i]);
        if (window && window->isOnDisplay()) {
            result = true;
        }

        i++;
    }

    return result;
}

bool MSceneManagerPrivate::canHaveAnimatedTransitions()
{
    Q_Q(MSceneManager);

#ifdef __arm__
    if (MApplication::softwareRendering()) {
        return false;
    }
#endif

    const QList<QGraphicsView *> viewsList = q->scene()->views();
    foreach (const QGraphicsView *view, viewsList) {
        const MWindow *window = qobject_cast<const MWindow *>(view);

        // We can have animated transitions if the MWindow is on display or
        // is still being initialized (didn't receive a visibility message
        // from xserver or window manager yet, probably because it's not being
        // backed up by a real top level X window yet).
        //
        // If it's visible in the switcher we don't animate as we shouldn't
        // be using many resources in this situation and the FPS is likely
        // to be limited to a fairly low number anyway.
        const bool animate = window
                             && (window->isOnDisplay() || !window->d_ptr->onDisplaySet)
                             && !window->isInSwitcher();
        if (animate) {
            return true;
        }
    }

    return false;
}

void MSceneManagerPrivate::produceMustBeResolvedDisplayEvent(MSceneWindow *sceneWindow)
{
    Q_Q(MSceneManager);

    QRectF viewRect(QPointF(0, 0), q->visibleSceneSize());
    MOnDisplayChangeEvent displayEvent(MOnDisplayChangeEvent::MustBeResolved, viewRect);

    scene->sendEvent(sceneWindow, &displayEvent);
}

void MSceneManagerPrivate::produceFullyOffDisplayEvents(QGraphicsItem *item)
{
    Q_Q(MSceneManager);
    MWidget *mWidget;

    if (item->isWidget()) {
        mWidget = qobject_cast<MWidget *>(static_cast<QGraphicsWidget *>(item));
        if (mWidget) {
            QRectF visibleSceneRect(QPoint(0, 0), q->visibleSceneSize());
            MOnDisplayChangeEvent event(MOnDisplayChangeEvent::FullyOffDisplay, visibleSceneRect);
            scene->sendEvent(mWidget, &event);
        }
    }

    QList<QGraphicsItem *> childItemsList = item->childItems();
    int childItemsCount = childItemsList.count();

    for (int i = 0; i < childItemsCount; i++) {
        produceFullyOffDisplayEvents(childItemsList.at(i));
    }
}

void MSceneManagerPrivate::produceSceneWindowEvent(QEvent::Type type,
        MSceneWindow *sceneWindow,
        bool animatedTransition)
{
    Q_Q(MSceneManager);
    MSceneWindowEvent event(type, sceneWindow, animatedTransition);
    QList<QGraphicsView *> viewsList = q->scene()->views();
    MApplicationWindow *window = 0;
    int numberOfViews = viewsList.count();

    // FIXME:
    // Actually sending the event would require overriding customEvent() which
    // would mess ABI compatibility. Calling the event handler directly for now

    if (type == MSceneWindowEvent::eventTypeAppear()) {
        for (int i = 0; i < numberOfViews; i++) {
            window = qobject_cast<MApplicationWindow *>(viewsList[i]);
            if (window) {
                window->d_func()->sceneWindowAppearEvent(&event);
            }
        }

    } else if (type == MSceneWindowEvent::eventTypeDisappear()) {
        for (int i = 0; i < numberOfViews; i++) {
            window = qobject_cast<MApplicationWindow *>(viewsList[i]);
            if (window) {
                window->d_func()->sceneWindowDisappearEvent(&event);
            }
        }
    } else if (type == MSceneWindowEvent::eventTypeDismiss()) {
        for (int i = 0; i < numberOfViews; i++) {
            window = qobject_cast<MApplicationWindow *>(viewsList[i]);
            if (window) {
                window->d_func()->sceneWindowDismissEvent(&event);
            }
        }
    } else {
        qFatal("Unknown event type in MSceneManagerPrivate::produceSceneWindowEvent");
    }
}

void MSceneManagerPrivate::updatePagesAndSheetsGeometry()
{
    foreach(MSceneWindow* window, windows) {
        if ((window->windowType() == MSceneWindow::Sheet || window->windowType() == MSceneWindow::ApplicationPage) &&
            window->sceneWindowState() != MSceneWindow::Disappeared)
        {
            setSceneWindowGeometry(window);
        }
    }
}

void MSceneManagerPrivate::addPagesAndSheetsGeometryUpdateAnimation(QParallelAnimationGroup* mainAnimation)
{
    foreach(MSceneWindow* window, windows) {
        if ((window->windowType() == MSceneWindow::Sheet || window->windowType() == MSceneWindow::ApplicationPage) &&
            window->sceneWindowState() != MSceneWindow::Disappeared)
        {
            QPropertyAnimation *geometryAnimation = new QPropertyAnimation;
            geometryAnimation->setTargetObject(window);
            geometryAnimation->setPropertyName("geometry");
            geometryAnimation->setStartValue(window->geometry());
            geometryAnimation->setEndValue(calculateSceneWindowGeometry(window));
            geometryAnimation->setEasingCurve(QEasingCurve::Linear);
            geometryAnimation->setDuration(300);
            mainAnimation->addAnimation(geometryAnimation);
        }
    }
}


void MSceneManagerPrivate::prepareWindowShow(MSceneWindow *window)
{
    Q_Q(MSceneManager);

    window->show();
    window->d_func()->dismissed = false;

    sceneWindowStack.add(window);

    setSceneWindowGeometry(window);
    setupLayerEffectForSceneWindow(window);

    if (window->windowType() == MSceneWindow::StatusBar) {
        // There can be only one status bar in the scene.
        Q_ASSERT(statusBar == 0);

        statusBar = window;
    }

    orientationAnimation->addSceneWindow(window);

    q->connect(window, SIGNAL(repositionNeeded()), SLOT(_q_setSenderGeometry()));
}

void MSceneManagerPrivate::startPageSwitchAnimation(MSceneWindow *newPage,
        MSceneWindow *oldPage,
        MPageSwitchAnimation::TransitionDirection direction)
{
    Q_ASSERT(pageSwitchAnimation);

    if (navigationBarAnimation)
        navigationBarAnimation->setTransitionDirection(direction == MPageSwitchAnimation::ToChildPage ?
                                                       MAbstractWidgetAnimation::In : MAbstractWidgetAnimation::Out);

    pageSwitchAnimation->setNewPage(newPage);
    pageSwitchAnimation->setOldPage(oldPage);
    pageSwitchAnimation->setTransitionDirection(direction);
    pageSwitchAnimation->start();

    freezeUIForAnimationDuration(pageSwitchAnimation);
}

void MSceneManagerPrivate::pushPage(MSceneWindow *page, bool animatedTransition)
{
    Q_Q(MSceneManager);
    MSceneWindow *previousPage = 0;

    // last chance to take a snapshot of navigationBar contents before it changes
    // due to eiter a change in the escape button state or presence (triggered from a
    // change in the page history, if the escape mode is in "auto") or finally due
    // to a change in the current page, which will cause the nav bar to be repopulated.
    if (currentPage && animatedTransition && navigationBarAnimation)
        navigationBarAnimation->takeContentSnapshot();

    if (currentPage && currentPage != page) {
        previousPage = currentPage;
        pageHistory.append(previousPage);
        emit q->pageHistoryChanged();
    }

    setCurrentPage(page);

    if (!pageHistory.empty() && animatedTransition && pageSwitchAnimation) {
        if (previousPage)
            setSceneWindowState(previousPage, MSceneWindow::Disappearing);

        setSceneWindowState(currentPage, MSceneWindow::Appearing);

        startPageSwitchAnimation(currentPage, previousPage, MPageSwitchAnimation::ToParentPage);
    } else {
        if (previousPage)
            setSceneWindowState(previousPage, MSceneWindow::Disappeared);

        setSceneWindowState(currentPage, MSceneWindow::Appeared);
    }
}

void MSceneManagerPrivate::popPage(bool animatedTransition)
{
    Q_Q(MSceneManager);
    MSceneWindow *previousPage = 0;
    bool pageHistoryChanged = false;

    // last chance to take a snapshot of navigationBar contents before it changes
    // due to eiter a change in the escape button state or presence (triggered from a
    // change in the page history, if the escape mode is in "auto") or finally due
    // to a change in the current page, which will cause the nav bar to be repopulated.
    if (animatedTransition && navigationBarAnimation)
        navigationBarAnimation->takeContentSnapshot();

    // Pages in the history might have been deleted overtime.
    while (previousPage == 0 && !pageHistory.isEmpty()) {
        previousPage = pageHistory.takeLast();
        pageHistoryChanged = true;

        if (!previousPage) {
            // Page has been deleted
            continue;
        }

        if (previousPage->d_func()->sceneManager != q)
            addSceneWindow(previousPage);

        if (previousPage->sceneWindowState() != MSceneWindow::Disappeared) {
            // We cannot use it.
            previousPage = 0;
        }
    }
    if (pageHistoryChanged)
        emit q->pageHistoryChanged();

    if (animatedTransition && pageSwitchAnimation) {
        if (previousPage)
            setSceneWindowState(previousPage, MSceneWindow::Appearing);

        setSceneWindowState(currentPage, MSceneWindow::Disappearing);

        startPageSwitchAnimation(previousPage, currentPage, MPageSwitchAnimation::ToChildPage);
    } else {
        if (previousPage)
            setSceneWindowState(previousPage, MSceneWindow::Appeared);

        setSceneWindowState(currentPage, MSceneWindow::Disappeared);
    }

    setCurrentPage(previousPage);
}

void MSceneManagerPrivate::setCurrentPage(QPointer<MSceneWindow> page)
{
    currentPage = page;

    if (!inputWidgetRelocator.isNull()) {
        inputWidgetRelocator->setCurrentPage(page);
    }
}

bool MSceneManagerPrivate::verifySceneWindowStateBeforeAppear(
        MSceneWindow *sceneWindow,
        MSceneWindow::DeletionPolicy policy,
        bool animatedTransition)
{
    bool verified = false;

    switch (sceneWindow->sceneWindowState()) {
        case MSceneWindow::Disappeared:
            if (sceneWindow->windowType() == MSceneWindow::ApplicationPage) {
                if (pageSwitchAnimation && pageSwitchAnimation->state() == QAbstractAnimation::Running) {
                    // Apply it after the current page switch animation finishes.
                    MSceneWindowTransition transition;
                    transition.sceneWindow = sceneWindow;
                    transition.type = MSceneWindowTransition::AppearTransition;
                    transition.policy = policy;
                    transition.animated = animatedTransition;
                    queuedTransitionsPageSwitchAnimation.append(transition);
                } else {
                    verified = true;
                }
            } else {
                // Correct source state. Proceed.
                verified = true;
            }
            break;

        case MSceneWindow::Disappearing:
            // Queue that transition.
            // Page transitions goes to a different queue than other scene windows.
            if (sceneWindow->windowType() == MSceneWindow::ApplicationPage) {
                Q_ASSERT(pageSwitchAnimation->state() == QAbstractAnimation::Running);

                // Apply it after the current page switch animation finishes.
                MSceneWindowTransition transition;
                transition.sceneWindow = sceneWindow;
                transition.type = MSceneWindowTransition::AppearTransition;
                transition.policy = policy;
                transition.animated = animatedTransition;
                queuedTransitionsPageSwitchAnimation.append(transition);
            } else {
                if (sceneWindow->d_func()->queuedTransition == 0) {
                    MSceneWindowTransition *queuedTransition = new MSceneWindowTransition;
                    queuedTransition->sceneWindow = sceneWindow;
                    queuedTransition->type = MSceneWindowTransition::AppearTransition;
                    queuedTransition->policy = policy;
                    queuedTransition->animated = animatedTransition;

                    sceneWindow->d_func()->queuedTransition = queuedTransition;
                } else {
                    // There's no sense in having a disappear() queued if it is already
                    // disappearing.
                    Q_ASSERT(sceneWindow->d_func()->queuedTransition->type ==
                            MSceneWindowTransition::AppearTransition);
                    // We already have an appear() call queued there. So nothing left to do.
                }
            }
            break;

        case MSceneWindow::Appearing:
            if (sceneWindow->windowType() == MSceneWindow::ApplicationPage) {
                // If there's any page being animated it means that a page switch animation is
                // taking place.
                Q_ASSERT(pageSwitchAnimation->state() == QAbstractAnimation::Running);

                // Remove all disappearances for this page from the queue
                foreach(MSceneWindowTransition transition, queuedTransitionsPageSwitchAnimation) {
                    if (transition.sceneWindow == sceneWindow
                        && transition.type == MSceneWindowTransition::DisappearTransition)
                    {
                        queuedTransitionsPageSwitchAnimation.removeAll(transition);
                        break;
                    }
                }

                if (!queuedTransitionsPageSwitchAnimation.isEmpty()) {
                    // something might be queded to replace us -- make sure we are on top afterwards
                    MSceneWindowTransition transition;
                    transition.sceneWindow = sceneWindow;
                    transition.type = MSceneWindowTransition::AppearTransition;
                    transition.policy = policy;
                    transition.animated = animatedTransition;
                    queuedTransitionsPageSwitchAnimation.append(transition);
                }
            } else {
                // This appear() request cancels the previously pending
                // disappear() or dismiss().
                if (sceneWindow->d_func()->queuedTransition != 0) {
                    Q_ASSERT(sceneWindow->d_func()->queuedTransition->type !=
                            MSceneWindowTransition::AppearTransition);

                    delete sceneWindow->d_func()->queuedTransition;
                    sceneWindow->d_func()->queuedTransition = 0;
                }
            }
            break;

        case MSceneWindow::Appeared:
            // We're already were we want to be.
            break;

        default:
            qFatal("MSceneManager - Unknown MSceneWindow::appearanceState().");
    }

    return verified;
}

void MSceneManagerPrivate::appearSceneWindow(MSceneWindow *window,
        MSceneWindow::DeletionPolicy policy,
        bool animatedTransition)
{
    Q_Q(MSceneManager);

    if (!verifySceneWindowStateBeforeAppear(window, policy, animatedTransition)) {
        return;
    }

    // Popping up scene windows during an orientation change is
    // a grey area. We probably want to avoid them during that period.
    // TODO: For now we are only queueing the status bar. We should
    // think about out a policy to apply to all scene windows.
    if (orientationAnimation->state() == QAbstractAnimation::Running
            && window->windowType() == MSceneWindow::StatusBar) {

        MSceneWindowTransition transition;
        transition.sceneWindow = window;
        transition.type = MSceneWindowTransition::AppearTransition;
        transition.policy = policy;
        transition.animated = animatedTransition;
        queuedTransitionsOrientationAnimation.append(transition);

        return;
    }

    window->d_func()->policy = policy;

    if (window->windowType() == MSceneWindow::ApplicationPage) {
        pushPage(window, animatedTransition);
    } else {
        if (animatedTransition) {
            setSceneWindowState(window, MSceneWindow::Appearing);
            if (!window->d_func()->appearanceAnimation)
                createAppearanceAnimationForSceneWindow(window);
            window->d_func()->appearanceAnimation->start();
            switch(window->windowType()) {
                case MSceneWindow::PopupList:
                case MSceneWindow::MessageBox:
                case MSceneWindow::Dialog:
                case MSceneWindow::ObjectMenu:
                case MSceneWindow::Sheet:
                    freezeUIForAnimationDuration(window->d_func()->appearanceAnimation);
                    // Fallthrough is intentional and freezeUIForAnimationDuration must be called
                    // before following connect since order of slot execution is crutial here.
                default:
                    q->connect(window->d_func()->appearanceAnimation, SIGNAL(finished()),
                               SLOT(_q_onSceneWindowAppearanceAnimationFinished()));
            }

        } else {
            setSceneWindowState(window, MSceneWindow::Appeared);
            if (window->windowType() == MSceneWindow::StatusBar) {
                qreal y = window->size().height();
                foreach(QGraphicsWidget *widget, rootElementsDisplacedByStatusBar)
                    widget->setPos(0, y);

                updatePagesAndSheetsGeometry();
            }
        }
    }

}

void MSceneManagerPrivate::prepareWindowHide(MSceneWindow *window)
{
    Q_Q(MSceneManager);

    sendCancelEvent(window);
    QObject::disconnect(window, SIGNAL(repositionNeeded()),
                        q, SLOT(_q_setSenderGeometry()));
    orientationAnimation->removeSceneWindow(window);
}

bool MSceneManagerPrivate::verifySceneWindowStateBeforeDisappear(
        MSceneWindow *sceneWindow,
        bool animatedTransition)
{
    bool verified = false;

    switch (sceneWindow->sceneWindowState()) {
        case MSceneWindow::Disappeared:
            // Nothing to do here.
            break;

        case MSceneWindow::Disappearing:
            if (sceneWindow->windowType() == MSceneWindow::ApplicationPage
                && pageSwitchAnimation->state() == QAbstractAnimation::Running) {
                foreach(MSceneWindowTransition transition, queuedTransitionsPageSwitchAnimation) {
                    if (transition.sceneWindow == sceneWindow
                        && transition.type == MSceneWindowTransition::AppearTransition)
                    {
                        queuedTransitionsPageSwitchAnimation.removeAll(transition);
                        break;
                    }
                }
            } else {
                // This disappear() request cancels the previously pending
                // appear()
                if (sceneWindow->d_func()->queuedTransition != 0) {
                    Q_ASSERT(sceneWindow->d_func()->queuedTransition->type ==
                            MSceneWindowTransition::AppearTransition);

                    delete sceneWindow->d_func()->queuedTransition;
                    sceneWindow->d_func()->queuedTransition = 0;
                }
            }
            break;

        case MSceneWindow::Appearing:
            // Queue that transition.
            // Page transitions goes to a different queue than other scene windows.
            if (sceneWindow->windowType() == MSceneWindow::ApplicationPage) {
                Q_ASSERT(pageSwitchAnimation && pageSwitchAnimation->state() == QAbstractAnimation::Running);

                // Apply it after the current page switch animation finishes.
                MSceneWindowTransition transition;
                transition.sceneWindow = sceneWindow;
                transition.type = MSceneWindowTransition::DisappearTransition;
                transition.animated = animatedTransition;
                queuedTransitionsPageSwitchAnimation.append(transition);
            } else {
                if (sceneWindow->d_func()->queuedTransition == 0) {
                    MSceneWindowTransition *queuedTransition = new MSceneWindowTransition;
                    queuedTransition->sceneWindow = sceneWindow;
                    queuedTransition->type = MSceneWindowTransition::DisappearTransition;
                    queuedTransition->animated = animatedTransition;

                    sceneWindow->d_func()->queuedTransition = queuedTransition;
                } else {
                    // There's no sense in having a appear() queued if it is already
                    // appearing.
                    Q_ASSERT(sceneWindow->d_func()->queuedTransition->type !=
                            MSceneWindowTransition::AppearTransition);
                }
            }
            break;

        case MSceneWindow::Appeared:
            if (sceneWindow->windowType() == MSceneWindow::ApplicationPage) {
                if (pageSwitchAnimation && pageSwitchAnimation->state() == QAbstractAnimation::Running) {
                    // Apply it after the current page switch animation finishes.
                    MSceneWindowTransition transition;
                    transition.sceneWindow = sceneWindow;
                    transition.type = MSceneWindowTransition::DisappearTransition;
                    transition.animated = animatedTransition;
                    queuedTransitionsPageSwitchAnimation.append(transition);
                } else {
                    verified = true;
                }
            } else {
                // Correct source state. Proceed.
                verified = true;
            }
            break;

        default:
            qFatal("MSceneManager - Unknown MSceneWindow::appearanceState().");
    }

    return verified;
}

void MSceneManagerPrivate::disappearSceneWindow(MSceneWindow *window,
        bool animatedTransition)
{
    Q_Q(MSceneManager);

    if (!verifySceneWindowStateBeforeDisappear(window, animatedTransition)) {
        return;
    }

    // Disappearing scene windows during an orientation change is
    // a grey area. We probably want to avoid them during that period.
    // TODO: For now we are only queueing the status bar. We should
    // think about out a policy to apply to all scene windows.
    if (orientationAnimation->state() == QAbstractAnimation::Running
            && window->windowType() == MSceneWindow::StatusBar) {

        MSceneWindowTransition transition;
        transition.sceneWindow = window;
        transition.type = MSceneWindowTransition::DisappearTransition;
        transition.animated = animatedTransition;
        queuedTransitionsOrientationAnimation.append(transition);

        return;
    }

    if (window->windowType() == MSceneWindow::ApplicationPage) {
        Q_ASSERT(window == currentPage);
        popPage(animatedTransition);

    } else if (animatedTransition) { // Fallback to legacy disappearance anim.
        setSceneWindowState(window, MSceneWindow::Disappearing);

        if (!window->d_func()->disappearanceAnimation)
            createDisappearanceAnimationForSceneWindow(window);

        window->d_func()->disappearanceAnimation->start();

        switch(window->windowType()) {
            case MSceneWindow::PopupList:
            case MSceneWindow::MessageBox:
            case MSceneWindow::Dialog:
            case MSceneWindow::ObjectMenu:
            case MSceneWindow::Sheet:
                freezeUIForAnimationDuration(window->d_func()->disappearanceAnimation);
            default:
                q->connect(window->d_func()->disappearanceAnimation, SIGNAL(finished()),
                        SLOT(_q_onSceneWindowDisappearanceAnimationFinished()));
        }
    } else {
        setSceneWindowState(window, MSceneWindow::Disappeared);
        if (window->windowType() == MSceneWindow::StatusBar) {
            foreach(QGraphicsWidget *widget, rootElementsDisplacedByStatusBar)
                widget->setPos(0, 0);

            updatePagesAndSheetsGeometry();
        }
    }
}

void MSceneManagerPrivate::freezeUIForAnimationDuration(QAbstractAnimation *animation)
{
    Q_Q(MSceneManager);
    eventEater->show();

    if (animation->state() == QAbstractAnimation::Running) {
        QObject::connect(animation,
                         SIGNAL(stateChanged(QAbstractAnimation::State,
                                             QAbstractAnimation::State)),
                         q, SLOT(_q_unfreezeUI(QAbstractAnimation::State,
                                               QAbstractAnimation::State)));
    }
}

MAbstractWidgetAnimation *MSceneManagerPrivate::createAnimationFromSceneWindowType(
        MSceneWindow* sceneWindow)
{
    MAbstractWidgetAnimation *animation = 0;

    switch(sceneWindow->windowType()) {
    case MSceneWindow::NotificationInformation:
        animation = new MWidgetSlideAnimation(sceneWindow);
        break;
    case MSceneWindow::NavigationBar:
        animation = qobject_cast<MAbstractWidgetAnimation*>(
                MTheme::animation(style()->navigationBarAnimation()));
        break;
    case MSceneWindow::NotificationEvent:
        animation = (sceneWindow->styleName() == "SystemBanner") ? qobject_cast<MAbstractWidgetAnimation*>(MTheme::animation(style()->systemBannerAnimation())) :
                                                                   qobject_cast<MAbstractWidgetAnimation*>(MTheme::animation(style()->notificationEventAnimation()));
        break;
    case MSceneWindow::ApplicationMenu:
        animation = qobject_cast<MAbstractWidgetAnimation*>(
                MTheme::animation(style()->applicationMenuAnimation()));
        break;
    case MSceneWindow::PopupList:
        animation = qobject_cast<MAbstractWidgetAnimation*>(
                MTheme::animation(style()->popupListAnimation()));
        break;
    case MSceneWindow::MessageBox:
        animation = qobject_cast<MAbstractWidgetAnimation*>(
                MTheme::animation(style()->messageBoxAnimation()));
        break;
    case MSceneWindow::Dialog:
        animation = qobject_cast<MAbstractWidgetAnimation*>(
                MTheme::animation(style()->dialogAnimation()));
        break;
    case MSceneWindow::ObjectMenu:
        animation = qobject_cast<MAbstractWidgetAnimation*>(
                MTheme::animation(style()->objectMenuAnimation()));
        break;
    case MSceneWindow::StatusBar: {
        animation = new MWidgetSlideAnimation(sceneWindow);
        break;
    }
    case MSceneWindow::Sheet: {
        animation = qobject_cast<MAbstractWidgetAnimation*>(
                    MTheme::animation(style()->sheetAnimation()));
        break;
    default:
        break;
    }
    }

    return animation;
}

void MSceneManagerPrivate::createAppearanceAnimationForSceneWindow(MSceneWindow *sceneWindow)
{
    Q_ASSERT(sceneWindow->d_func()->appearanceAnimation == 0);
    MAbstractWidgetAnimation *animation = 0;

    // 1. First get the animation from the scene window style itself.
    QString appearanceAnimationName = sceneWindow->property("_m_appearanceAnimation").toString();
    if (!appearanceAnimationName.isEmpty()) {
        animation = qobject_cast<MAbstractWidgetAnimation*>(
                MTheme::animation(appearanceAnimationName));
    }

    // 2. If not defined, try to get it from our own style.
    if (!animation)
        animation = createAnimationFromSceneWindowType(sceneWindow);

    // FIXME: Find a better place for this logic. Or find a better solution for this problem.
    if (sceneWindow->windowType() == MSceneWindow::StatusBar &&
        qobject_cast<MWidgetSlideAnimation*>(animation)) {

        foreach(QGraphicsWidget *widget, rootElementsDisplacedByStatusBar) {
            MWidgetMoveAnimation *moveAnimation = new MWidgetMoveAnimation;
            moveAnimation->setWidget(widget);
            moveAnimation->setFinalPos(QPointF(0, statusBar->size().height()));
            animation->addAnimation(moveAnimation);
        }

        addPagesAndSheetsGeometryUpdateAnimation(animation);
    }
    // end of FIXME

    // 3. As a last resort, use hard coded animation assignment.
    if (!animation)
        animation = new MWidgetFadeAnimation(sceneWindow);

    animation->setProperty("origin", sceneWindow->boundingRect().center());
    animation->setTransitionDirection(MAbstractWidgetAnimation::In);
    animation->setTargetWidget(sceneWindow);
    animation->setParent(sceneWindow);

    MSceneWindow *effect = sceneWindow->d_func()->effect;
    if (effect)
        animation->addAnimation(effect->d_func()->appearanceAnimation);

    sceneWindow->d_func()->appearanceAnimation = animation;
}

void MSceneManagerPrivate::createDisappearanceAnimationForSceneWindow(MSceneWindow *sceneWindow)
{
    Q_ASSERT(sceneWindow->d_func()->disappearanceAnimation == 0);
    MAbstractWidgetAnimation *animation = 0;

    // 1. First get the animation from the scene window style itself.
    QString disappearanceAnimationName =
            sceneWindow->property("_m_disappearanceAnimation").toString();
    if (!disappearanceAnimationName.isEmpty()) {
        animation = qobject_cast<MAbstractWidgetAnimation*>(
                MTheme::animation(disappearanceAnimationName));
    }

    // 2. If not defined, try to get it from our own style.
    if (!animation)
        animation = createAnimationFromSceneWindowType(sceneWindow);

    // FIXME: Find a better place for this logic. Or find a better solution for this problem.
    if (sceneWindow->windowType() == MSceneWindow::StatusBar &&
        qobject_cast<MWidgetSlideAnimation*>(animation)) {

        foreach(QGraphicsWidget *widget, rootElementsDisplacedByStatusBar) {
            // OBS: for this to look correct both status bar slide animation
            // and our root elements move animations must have the same
            // parameters (easing curve, duration...)
            MWidgetMoveAnimation *moveAnimation = new MWidgetMoveAnimation;
            moveAnimation->setWidget(widget);
            moveAnimation->setFinalPos(QPointF(0, 0));
            animation->addAnimation(moveAnimation);
        }

        addPagesAndSheetsGeometryUpdateAnimation(animation);
    }
    // end of FIXME

    // 3. As a last resort, use hard coded animation assignment.
    if (!animation)
        animation = new MWidgetFadeAnimation(sceneWindow);

    animation->setProperty("origin", sceneWindow->boundingRect().center());
    animation->setTransitionDirection(MAbstractWidgetAnimation::Out);
    animation->setTargetWidget(sceneWindow);
    animation->setParent(sceneWindow);

    MSceneWindow *effect = sceneWindow->d_func()->effect;
    if (effect)
        animation->addAnimation(effect->d_func()->disappearanceAnimation);

    sceneWindow->d_func()->disappearanceAnimation = animation;
}

void MSceneManagerPrivate::_q_updateRootElementsPositions()
{
    qreal y;

    if (statusBar) {
        y = statusBar->size().height();
    } else {
        y = 0;
    }

    foreach(QGraphicsWidget *rootElement, rootElementsDisplacedByStatusBar)
        rootElement->setPos(0, y);
}

void MSceneManagerPrivate::setSceneWindowState(MSceneWindow *sceneWindow,
        MSceneWindow::SceneWindowState newState)
{
    const MSceneWindow::SceneWindowState oldState = sceneWindow->sceneWindowState();

    switch (newState) {
        case MSceneWindow::Appearing:
            onSceneWindowEnteringAppearingState(sceneWindow);
            break;

        case MSceneWindow::Appeared:
            onSceneWindowEnteringAppearedState(sceneWindow);
            break;

        case MSceneWindow::Disappearing:
            onSceneWindowEnteringDisappearingState(sceneWindow);
            break;

        case MSceneWindow::Disappeared:
            onSceneWindowEnteringDisappearedState(sceneWindow);
            break;
    }

    sceneWindow->d_func()->setSceneWindowState(newState);

#ifdef Q_WS_X11
    if ((sceneWindow->sceneWindowState() == MSceneWindow::Appeared ||
         sceneWindow->sceneWindowState() == MSceneWindow::Disappeared) &&
        sceneWindow->zValue() >= MSceneManagerPrivate::EscapeButtonPanel)
    {
        _q_updateDecoratorButtonsProperty();
    }

    if (sceneWindow->windowType() == MSceneWindow::StatusBar &&
        (sceneWindow->sceneWindowState() == MSceneWindow::Appeared ||
         sceneWindow->sceneWindowState() == MSceneWindow::Disappeared))
    {
        updateStatusBarGeometryProperty();
    }
#endif

    if (newState != oldState) {
        inputWidgetRelocator->sceneWindowStateHasChanged(sceneWindow, newState, oldState);
    }
}

void MSceneManagerPrivate::onSceneWindowEnteringAppearingState(MSceneWindow *sceneWindow)
{
    prepareWindowShow(sceneWindow);
    produceSceneWindowEvent(MSceneWindowEvent::eventTypeAppear(), sceneWindow, true);
}

void MSceneManagerPrivate::onSceneWindowEnteringAppearedState(MSceneWindow *sceneWindow)
{
    Q_Q(MSceneManager);

    switch (sceneWindow->sceneWindowState()) {
    case MSceneWindow::Appearing:
        // Page switching uses a separate animation.
        // Thus in this case appearanceAnimation == 0.
        if (sceneWindow->d_func()->appearanceAnimation != 0) {
            delete sceneWindow->d_func()->appearanceAnimation;
            sceneWindow->d_func()->appearanceAnimation = 0;
        }
        break;

    case MSceneWindow::Disappeared:
        prepareWindowShow(sceneWindow);
        produceSceneWindowEvent(MSceneWindowEvent::eventTypeAppear(), sceneWindow, false);
        break;

    default:
        break;
    }

    // Make sure that items with subfocus get the scene's focus after
    // appearing:
    QGraphicsItem *nextFocusItem = sceneWindow->focusItem();
    if (nextFocusItem && !nextFocusItem->hasFocus()) {
        scene->setFocusItem(nextFocusItem, Qt::OtherFocusReason);
    }

    MSceneWindow::WindowType type = sceneWindow->windowType();

    if (type == MSceneWindow::Dialog ||
        type == MSceneWindow::MessageBox ||
        type == MSceneWindow::ApplicationMenu ||
        type == MSceneWindow::ObjectMenu ||
        type == MSceneWindow::ModalSceneWindow ||
        type == MSceneWindow::PopupList ||
        type == MSceneWindow::Sheet) {

        // Notify page on first blocker appearing
        if (currentPage && blockingWindows.count() == 0) {
            QEvent e(QEvent::WindowBlocked);
            qApp->sendEvent(currentPage, &e);
        }

        blockingWindows.append(sceneWindow);
    }

    if (type == MSceneWindow::Sheet) {
        // A sheet by definition takes the entire screen and is fully opaque.
        // Therefore all scene windows behind it won't be visible at all.
        // But they will still be painted.
        // As an optimization we hide them all.
        updateVisibilityOfSceneWindowsBehind(sceneWindow, false);
    }

    if (isOnDisplay()) {
        produceMustBeResolvedDisplayEvent(sceneWindow);
    }

    if (sceneWindow->windowType() == MSceneWindow::EscapeButtonPanel) {
        MEscapeButtonPanel *escapeButtonPanel = static_cast<MEscapeButtonPanel*>(sceneWindow);
        q->connect(escapeButtonPanel, SIGNAL(escapeModeChanged(MEscapeButtonPanelModel::EscapeMode)),
                   SLOT(_q_updateDecoratorButtonsProperty()));
    }
}

void MSceneManagerPrivate::onSceneWindowEnteringDisappearingState(MSceneWindow *sceneWindow)
{
    if (sceneWindow->windowType() == MSceneWindow::Sheet
        && sceneWindow == findTopMostSheet()) {
        // A sheet by definition takes the entire screen and is fully opaque.
        // Therefore all scene windows behind it won't be visible at all.
        // But they will still be painted.
        // As an optimization we have hidden them all.
        // Now we should restore their visibilities.
        updateVisibilityOfSceneWindowsBehind(sceneWindow, true);
    }

    if (sceneWindow->d_func()->dismissed) {
        produceSceneWindowEvent(MSceneWindowEvent::eventTypeDismiss(),
                sceneWindow, true);
    } else {
        produceSceneWindowEvent(MSceneWindowEvent::eventTypeDisappear(),
                sceneWindow, true);
    }

    prepareWindowHide(sceneWindow);
}

void MSceneManagerPrivate::onSceneWindowEnteringDisappearedState(MSceneWindow *sceneWindow)
{
    Q_Q(MSceneManager);

    switch (sceneWindow->sceneWindowState()) {
        case MSceneWindow::Appearing:
            // Happens if removeSceneWindow() is called during an appearance animation
            {
                MAbstractWidgetAnimation *appearanceAnimation =
                    sceneWindow->d_func()->appearanceAnimation;

                if (appearanceAnimation) {
                    appearanceAnimation->stop();
                    appearanceAnimation->restoreTargetWidgetState();
                    delete appearanceAnimation;
                    sceneWindow->d_func()->appearanceAnimation = 0;
                }
            }
            break;

        case MSceneWindow::Disappearing:
            {
                MAbstractWidgetAnimation *disappearanceAnimation =
                    sceneWindow->d_func()->disappearanceAnimation;

                if (disappearanceAnimation) {
                    disappearanceAnimation->stop();
                    disappearanceAnimation->restoreTargetWidgetState();
                    delete disappearanceAnimation;
                    sceneWindow->d_func()->disappearanceAnimation = 0;
                }
            }
            break;

        case MSceneWindow::Appeared:

            if (sceneWindow->windowType() == MSceneWindow::Sheet
                && sceneWindow == findTopMostSheet()) {
                // A sheet by definition takes the entire screen and is fully opaque.
                // Therefore all scene windows behind it won't be visible at all.
                // But they will still be painted.
                // As an optimization we have hidden them all.
                // Now we should restore their visibilities.
                updateVisibilityOfSceneWindowsBehind(sceneWindow, true);
            }

            produceSceneWindowEvent(MSceneWindowEvent::eventTypeDisappear(),
                    sceneWindow, false);
            prepareWindowHide(sceneWindow);
            break;

        default:
            break;
    }

    MSceneWindow::WindowType type = sceneWindow->windowType();

    if (type == MSceneWindow::Dialog ||
        type == MSceneWindow::MessageBox ||
        type == MSceneWindow::ApplicationMenu ||
        type == MSceneWindow::ObjectMenu ||
        type == MSceneWindow::ModalSceneWindow ||
        type == MSceneWindow::PopupList ||
        type == MSceneWindow::Sheet) {

        blockingWindows.removeOne(sceneWindow);

        // Notify page on last blocker having disappeared
        if (currentPage && blockingWindows.count() == 0) {
            QEvent e(QEvent::WindowUnblocked);
            qApp->sendEvent(currentPage, &e);
        }
    }

    if (currentPage == sceneWindow)
        setCurrentPage(0);

    if (sceneWindow->windowType() == MSceneWindow::StatusBar)
        statusBar = 0;


    if (sceneWindow->windowType() == MSceneWindow::EscapeButtonPanel) {
        MEscapeButtonPanel *escapeButtonPanel = qobject_cast<MEscapeButtonPanel*>(sceneWindow);
        if (escapeButtonPanel)
            q->disconnect(escapeButtonPanel, SIGNAL(escapeModeChanged(MEscapeButtonPanelModel::EscapeMode)),
                q, SLOT(_q_updateDecoratorButtonsProperty()));
    }

    _q_undoSceneWindowDislocation(sceneWindow);

    // If there is a layer effect it is deleted
    destroyLayerEffectForSceneWindow(sceneWindow);

    sceneWindowStack.remove(sceneWindow);

    sceneWindow->hide();

    if (sceneWindow->sceneWindowState() == MSceneWindow::Disappearing ||
            sceneWindow->sceneWindowState() == MSceneWindow::Appeared) {

        if (isOnDisplay())
            produceFullyOffDisplayEvents(sceneWindow);

        if ((sceneWindow->deletionPolicy() == MSceneWindow::DestroyWhenDone) ||
                ((sceneWindow->deletionPolicy() == MSceneWindow::DestroyWhenDismissed)
                 && sceneWindow->d_func()->dismissed)) {
            sceneWindow->deleteLater();
        }
    }
}

void MSceneManagerPrivate::sendCancelEvent(MSceneWindow *window)
{
    Q_Q(MSceneManager);

    QGraphicsItem *mouseGrabber;
    mouseGrabber = q->scene()->mouseGrabberItem();

    if (mouseGrabber
        && (window == mouseGrabber || window->isAncestorOf(mouseGrabber))) {
        MCancelEvent cancelEvent;
        q->scene()->sendEvent(mouseGrabber, &cancelEvent);
    }
}

void MSceneManagerPrivate::fastForwardSceneWindowTransitionAnimation(MSceneWindow *sceneWindow)
{
    if (sceneWindow->windowType() == MSceneWindow::ApplicationPage) {
        if (pageSwitchAnimation->oldPage() == sceneWindow ||
                pageSwitchAnimation->newPage() == sceneWindow)
            pageSwitchAnimation->setCurrentTime(pageSwitchAnimation->duration());
    } else {
        if (sceneWindow->sceneWindowState() == MSceneWindow::Appearing) {
            sceneWindow->d_func()->appearanceAnimation->setCurrentTime(
                    sceneWindow->d_func()->appearanceAnimation->duration());
        } else if (sceneWindow->sceneWindowState() == MSceneWindow::Disappearing) {
            sceneWindow->d_func()->disappearanceAnimation->setCurrentTime(
                    sceneWindow->d_func()->disappearanceAnimation->duration());
        }
    }
}

MSceneManagerStyleContainer &MSceneManagerPrivate::style()
{
    Q_Q(MSceneManager);

    if (!styleContainer) {
        styleContainer = createStyleContainer();
        styleContainer->initialize(q->objectName(), "", NULL);
    }

    return *styleContainer;
}

const MSceneManagerStyleContainer &MSceneManagerPrivate::style() const
{
    Q_Q(const MSceneManager);

    if (!styleContainer) {
        MSceneManagerPrivate *d_p = const_cast<MSceneManagerPrivate *>(this);
        d_p->styleContainer = createStyleContainer();
        d_p->styleContainer->initialize(q->objectName(), "", NULL);
    }

    return *styleContainer;
}

MSceneManagerStyleContainer *MSceneManagerPrivate::createStyleContainer() const
{
    return new MSceneManagerStyleContainer();
}

MSceneWindow *MSceneManagerPrivate::findTopMostSheet()
{
    const QList<MSceneWindow *> &stackList = sceneWindowStack.list();
    MSceneWindow *sheet = 0;
    MSceneWindow *sceneWindow;
    int i = stackList.count() - 1;

    while (!sheet && i >= 0) {
        sceneWindow = stackList.at(i);
        if (sceneWindow->windowType() == MSceneWindow::Sheet) {
            sheet = sceneWindow;
        } else {
            --i;
        }
    }

    return sheet;
}

void MSceneManagerPrivate::updateVisibilityOfSceneWindowsBehind(
        MSceneWindow *referenceSceneWindow, bool newVisibility)
{
    MSceneWindow *sceneWindow;
    const QList<MSceneWindow *> &stackList = sceneWindowStack.list();

    for (int i = stackList.indexOf(referenceSceneWindow) - 1; i >= 0; --i) {
        sceneWindow = stackList.at(i);
        sceneWindow->setVisible(newVisibility);

        if (sceneWindow->windowType() == MSceneWindow::Sheet
            && sceneWindow->isVisible()) {
            // don't propagate the visibility further as the sheet
            // obscures all scene windows behind it.
            break;
        }
    }
}

MSceneManager::MSceneManager(MScene *scene, QObject *parent) :
    QObject(parent), d_ptr(new MSceneManagerPrivate)
{
    Q_D(MSceneManager);

    if (scene == 0)
        scene = new MScene(this);

    d->q_ptr = this;
    d->init(scene);
    d->scene->d_ptr->setSceneManager(this);
}

MSceneManager::~MSceneManager()
{
    Q_D(MSceneManager);

    delete d;
}

MScene *MSceneManager::scene()
{
    Q_D(MSceneManager);

    return d->scene;
}

void MSceneManager::appearSceneWindow(MSceneWindow *window, MSceneWindow::DeletionPolicy policy)
{
    Q_D(MSceneManager);

    d->addSceneWindow(window);

    d->appearSceneWindow(window, policy, d->canHaveAnimatedTransitions());
}

void MSceneManager::appearSceneWindowNow(MSceneWindow *window, MSceneWindow::DeletionPolicy policy)
{
    Q_D(MSceneManager);
    d->addSceneWindow(window);
    d->appearSceneWindow(window, policy, false);
}

int MSceneManager::execDialog(MDialog *dialog)
{
    if (dialog == 0) {
        mWarning("MSceneManager") << "Invalid dialog instance";
        return MDialog::Rejected;
    }

    QEventLoop eventLoop;
    QPointer<MDialog> dialog_ptr = dialog;
    connect(dialog, SIGNAL(finished(int)), &eventLoop, SLOT(quit()));
    connect(dialog, SIGNAL(destroyed()), &eventLoop, SLOT(quit()));

    //TODO: Figure better workaround for this (or ask Qt to
    //      fix this for us).
    //
    //Launching of a modal scene window during mouse event handling
    //(mouse press/release callbacks etc.) will block the mainloop and
    //might break the event handling.

    //Release mouse focus from the current mousegrabber, so that the first
    //mouse event wont be sent to wrong widget. This is needed if a
    //modal scene window was executed during mouse release event.
    QGraphicsItem *g = scene()->mouseGrabberItem();
    if (g)
        g->ungrabMouse();

    appearSceneWindow(dialog);
    eventLoop.exec();

    // use QPointer in case of the dialog being deleted in the meantime
    if (dialog_ptr) {
        MButtonModel *clickedButton = dialog->clickedButton();
        if (clickedButton)
            return dialog->standardButton(clickedButton);
        else
            return dialog->result();
    }

    return MDialog::Rejected;
}

void MSceneManager::disappearSceneWindow(MSceneWindow *window)
{
    Q_D(MSceneManager);

    if (!window->d_func()->canDisappear())
        return;

    window->d_func()->dismissed = false;
    d->disappearSceneWindow(window, d->canHaveAnimatedTransitions());
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *window)
{
    Q_D(MSceneManager);

    if (!window->d_func()->canDisappear())
        return;

    window->d_func()->dismissed = false;
    d->disappearSceneWindow(window, false);
}

void MSceneManager::dismissSceneWindow(MSceneWindow *window)
{
    Q_D(MSceneManager);

    if (!window->d_func()->canDismiss())
        return;

    window->d_func()->dismissed = true;
    d->disappearSceneWindow(window, d->canHaveAnimatedTransitions());
}

void MSceneManager::dismissSceneWindowNow(MSceneWindow *window)
{
    Q_D(MSceneManager);

    if (!window->d_func()->canDismiss())
        return;

    window->d_func()->dismissed = true;
    d->disappearSceneWindow(window, false);
}

bool MSceneManager::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);

    event->ignore();
    return false;
}

void MSceneManager::requestSoftwareInputPanel(QGraphicsWidget *)
{
    MInputMethodState::requestSoftwareInputPanel();

    // This is normally called automatically except in cases where input panel area does not change and we
    // move between two similar text edits, for example. This should be removed if we ever get a change to
    // track focus item changes in the scene (see QTBUG-10570).
    // Calling update immediately here may cause relocated widget not to be set to its optimal position.
    // However, it is guaranteed to be visible on the screen, optimally or not.
    ensureCursorVisible();
}

void MSceneManager::closeSoftwareInputPanel()
{
    MInputMethodState::closeSoftwareInputPanel();
}

void MSceneManager::setOrientationAngle(M::OrientationAngle angle,
                                          TransitionMode mode)
{
    Q_D(MSceneManager);

    if (d->orientationAnimation->state() == QAbstractAnimation::Running) {
        // Don't stop the current animation, instead remember the new
        // animation as pending and start it after the current animation
        // has been finished (see _q_applyPendingOrientationChange()).
        if (d->newAngle != angle) {
            if (d->pendingRotation == 0) {
                d->pendingRotation = new MSceneManagerPrivate::Rotation;
            }
            d->pendingRotation->angle = angle;
            d->pendingRotation->mode = mode;
        } else {
            // We are already transitioning to this angle.
            // Thus no need for a pending rotation.
            if (d->pendingRotation) {
                delete d->pendingRotation;
                d->pendingRotation = 0;
            }
        }
    } else {
        // In order to enable a synchronized rotation of app and keyboard,
        // meego-im-uiserver captures an app screenshot, blends it
        // with the keyboard and displays that animation on top
        // of anything else that's on the screen. The application needs
        // to rotate quickly, without animation. This patch
        // suppresses the rotation animation on the application side
        // when VKB is shown - see also NB#231633.
        // i.e., the virtual keyboard does the composition of application
        // window + virtual keyboard by itself.
        if (mode == AnimatedTransition && d->canHaveAnimatedTransitions()
            && MInputMethodState::instance()->inputMethodArea().isNull()) {
            d->rotateToAngle(angle);
        } else {
            d->setOrientationAngleWithoutAnimation(angle);
        }
    }
}

M::Orientation MSceneManager::orientation() const
{
    Q_D(const MSceneManager);

    return MDeviceProfile::instance()->orientationFromAngle(d->angle);
}

M::OrientationAngle MSceneManager::orientationAngle() const
{
    Q_D(const MSceneManager);

    return d->angle;
}

QSize MSceneManager::visibleSceneSize(M::Orientation orientation) const
{
    QSize s;

    if (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Landscape) {
        // native display orientation is landscape
        if (orientation == M::Landscape)
            s = QSize(MDeviceProfile::instance()->resolution().width(),
                      MDeviceProfile::instance()->resolution().height());
        else
            s = QSize(MDeviceProfile::instance()->resolution().height(),
                      MDeviceProfile::instance()->resolution().width());
    } else {
        // native display orientation is portrait
        if (orientation == M::Landscape)
            s = QSize(MDeviceProfile::instance()->resolution().height(),
                      MDeviceProfile::instance()->resolution().width());
        else
            s = QSize(MDeviceProfile::instance()->resolution().width(),
                      MDeviceProfile::instance()->resolution().height());
    }

    return s;
}

QSize MSceneManager::visibleSceneSize() const
{
    return visibleSceneSize(orientation());
}

QList<MSceneWindow *> MSceneManager::pageHistory() const
{
    Q_D(const MSceneManager);
    QList<MSceneWindow *> cleanList;
    int pageCount = d->pageHistory.count();
    MSceneWindow *page;

    // We might have some null entries in the history.
    for (int i = 0; i < pageCount; i++) {
        page = d->pageHistory.at(i);
        if (page) {
            cleanList.append(page);
        }
    }

    return cleanList;
}

void MSceneManager::setPageHistory(const QList<MSceneWindow *> &list)
{
    Q_D(MSceneManager);
    int pageCount = list.count();
    QList<MSceneWindow *> currentPageHistory = pageHistory();

    if (currentPageHistory == list) {
        // Nothing changes, thus nothing to be done.
        return;
    }

    d->pageHistory.clear();

    for (int i = 0; i < pageCount; i++) {
        d->pageHistory.append(list.at(i));
    }

    emit pageHistoryChanged();
}

void MSceneManager::childEvent(QChildEvent *event)
{
    Q_UNUSED(event);
}

void MSceneManager::fastForwardPageSwitchAnimation()
{
    Q_D(MSceneManager);
    d->pageSwitchAnimation->setCurrentTime(d->pageSwitchAnimation->duration());
}

void MSceneManager::fastForwardOrientationChangeAnimation()
{
    Q_D(MSceneManager);
    d->orientationAnimation->setCurrentTime(d->orientationAnimation->duration());
}

void MSceneManager::fastForwardSceneWindowTransitionAnimation(MSceneWindow *sceneWindow)
{
    Q_D(MSceneManager);
    d->fastForwardSceneWindowTransitionAnimation(sceneWindow);
}

void MSceneManager::addSceneWindow(MSceneWindow *sceneWindow)
{
    Q_D(MSceneManager);
    d->addSceneWindow(sceneWindow);
}

void MSceneManager::removeSceneWindow(MSceneWindow *sceneWindow)
{
    Q_D(MSceneManager);
    d->removeSceneWindow(sceneWindow);
}

#include "moc_mscenemanager.cpp"
