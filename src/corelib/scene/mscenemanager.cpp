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

#include "mondisplaychangeevent.h"
#include "morientationchangeevent.h"
#include "mscenewindow.h"
#include "mscenewindow_p.h"
#include "mdeviceprofile.h"
#include "mdialog.h"
#include "mscene.h"
#include "mscene_p.h"
#include "mscenelayereffect.h"
#include "mnavigationbar.h"
#include "mdockwidget.h"
#include "mobjectmenu.h"
#include "mapplication.h"
#include "mwindow.h"
#include "mapplicationwindow.h"
#include "mapplicationwindow_p.h"
#include "mpannableviewport.h"
#include "mtextedit.h"

#include "minputmethodstate.h"

#include "mapplicationpage.h"
#include "morientationtracker.h"

#include "mbasicorientationanimation.h"
#include "mcrossfadedorientationanimation.h"
#include "mabstractwidgetanimation.h"
#include "mpageswitchanimation.h"
#include "msceneeventeater.h"

#include <mwidgetslideanimation.h>
#include <mwidgetfadeanimation.h>
#include <mwidgetzoomanimation.h>
#include <mwidgetmoveanimation.h>

namespace
{
    //! This is a guess for the SIP close timeout since there is no general way of getting it.
    const int SoftwareInputPanelHideTimer = 500;
}


void MSceneManagerPrivate::init(MScene *scene)
{
    Q_Q(MSceneManager);

    this->scene = scene;

    currentPage = 0;

    focusedInputWidget = 0;
    alteredSceneWindow = 0;
    sceneWindowTranslation = QPoint();

    statusBar = 0;

    pendingRotation = 0;

    initOrientationAngles();

    rootElement = new QGraphicsWidget();
    homeButtonRootElement = new QGraphicsWidget(rootElement);
    homeButtonRootElement->setZValue(zForWindowType(MSceneWindow::HomeButtonPanel));
    navigationBarRootElement = new QGraphicsWidget(rootElement);
    navigationBarRootElement->setZValue(zForWindowType(MSceneWindow::NavigationBar));
    rootElement->setTransformOriginPoint(QPointF(q->visibleSceneSize().width() / 2.0, q->visibleSceneSize().height() / 2.0));
    scene->addItem(rootElement);

    eventEater = new MSceneEventEater();
    scene->addItem(eventEater);
    eventEater->setGeometry(QRectF(QPointF(0.0, 0.0), q->visibleSceneSize(M::Landscape)));
    createOrientationAnimation();

    // The scene manager should only listen to region updates from one instance, to prevent
    // conflicting window relocation requests. Since MIMS is a singleton, enforcing
    // Qt::UniqueConnection is sufficient.
    // This also implies that the software input panel could come up without an explicit request
    // from the application side.
    q->connect(MInputMethodState::instance(), SIGNAL(inputMethodAreaChanged(QRect)),
               q, SLOT(_q_inputPanelAreaChanged(const QRect &)),
               Qt::UniqueConnection);

    q->connect(q, SIGNAL(orientationChangeFinished(M::Orientation)),
               q, SLOT(ensureCursorVisible()),
               Qt::UniqueConnection);

    pageSwitchAnimation = new MPageSwitchAnimation;
    q->connect(pageSwitchAnimation, SIGNAL(finished()),
            SLOT(_q_onPageSwitchAnimationFinished()));

    setOrientationAngleWithoutAnimation(newAngle);
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

    QRectF visibleSceneRect = QRectF(QPointF(0.0, 0.0), q->visibleSceneSize(M::Landscape));

    //TODO: get this from theme
    orientationAnimation = new MCrossFadedOrientationAnimation(visibleSceneRect);
    //orientationAnimation = new MBasicOrientationAnimation(q->visibleSceneSize(M::Landscape));

    orientationAnimation->setRootElement(rootElement);

    q->connect(orientationAnimation, SIGNAL(orientationChanged()), SLOT(_q_changeGlobalOrientationAngle()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_emitOrientationChangeFinished()));
    q->connect(orientationAnimation, SIGNAL(finished()),
            SLOT(_q_applySceneWindowTransitionsQueuedDueToOrientationAnimation()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_triggerAsyncPendingOrientationChange()));
}

MSceneManagerPrivate::~MSceneManagerPrivate()
{
    delete orientationAnimation;
    delete pageSwitchAnimation;
    delete pendingRotation;
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
    M::Orientation oldOrientation = orientation(angle);
    angle = newAngle;

    if (oldOrientation != orientation(angle)) {
        emit q->orientationAboutToChange(q->orientation());

        notifyWidgetsAboutOrientationChange();
        setSceneWindowGeometries();

        // emit signal after sending the orientation event to widgets (in case someone
        // would like to connect to the signal and get correct size hints for widgets)
        emit q->orientationChanged(q->orientation());
    }
    if (oldAngle != angle)
        emit q->orientationAngleChanged(angle);
}

void MSceneManagerPrivate::_q_emitOrientationChangeFinished()
{
    Q_Q(MSceneManager);

    emit q->orientationChangeFinished(q->orientation());
}

void MSceneManagerPrivate::_q_unFreezeUI()
{
    Q_Q(MSceneManager);
    eventEater->hide();

    QAbstractAnimation *animation = dynamic_cast<QAbstractAnimation *>(q->sender());
    if (animation == 0)
        return;

    QObject::disconnect(animation, SIGNAL(finished()), q, SLOT(_q_unFreezeUI()));
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

void MSceneManagerPrivate::_q_inputPanelAreaChanged(const QRect &panelRect)
{
    if (panelRect.isEmpty()) {
        restoreSceneWindow();
    } else {
        relocateWindowByInputPanel(panelRect);
    }
}

void MSceneManagerPrivate::relocateWindowByInputPanel(const QRect &inputPanelRect)
{
    Q_Q(MSceneManager);

    const QRectF microFocusRect = q->scene()->inputMethodQuery(Qt::ImMicroFocus).toRectF();
    // If the microfocus is invalid then the focused widget does not want to be relocated.
    if (!microFocusRect.isValid()) {
        return;
    }

    // This method is not responsible for restoring visibility when the input panel is closed -
    // restoreSceneWindow() does that. Therefore, it is OK to also ignore empty rectangles here.
    if (!focusedInputWidget || inputPanelRect.isEmpty()) {
        return;
    }

    // Find the first scene window parent of the focused input widget, since only those should
    // be scrolled or moved in this context.
    MSceneWindow *newParent = parentSceneWindow(focusedInputWidget);

    // Don't move navigation bar. Focus can also be in dock widget but because it can get
    // obfuscated by input panel we need to move it.
    if (newParent->windowType() == MSceneWindow::NavigationBar) {
        return;
    }

    // The visible scene size is already mapped to the rootElement's coordinate system.
    const QRect screenSizeRect(QPoint(0, 0), q->visibleSceneSize());
    const QRect mappedRect(rootElement->mapRectFromScene(inputPanelRect).toRect());

    // Need to also handle the case where part of the input panel is outside of the visible scene
    // size, hence the intersection. It is also assumed that the input panel always pops up from
    // the bottom of the visible scene area.
    const int obstructedHeight(mappedRect.intersect(screenSizeRect).height());
    QRect visibleRect(screenSizeRect);
    visibleRect.setHeight(visibleRect.height() - obstructedHeight);

    // Always try to center the input focus into the remaining visible rectangle.
    int adjustment = (rootElement->mapRectFromScene(microFocusRect).toRect().center() -
                      visibleRect.center()).y();

    // The altered scene window is moved back to its
    // original location, although - if it has pannable contents - it is not scrolled back.
    adjustment -= scrollPageContents(newParent, adjustment);
    moveSceneWindow(newParent, adjustment, obstructedHeight);
}

void MSceneManagerPrivate::restoreSceneWindow()
{
    if (alteredSceneWindow) {
        sceneWindowTranslation *= -1;
        alteredSceneWindow->moveBy(sceneWindowTranslation.x(), sceneWindowTranslation.y());
        sceneWindowTranslation = QPoint();
        alteredSceneWindow = 0;
    }
}

void MSceneManager::ensureCursorVisible()
{
    Q_D(MSceneManager);

    d->_q_inputPanelAreaChanged(MInputMethodState::instance()->inputMethodArea());
}

M::Orientation MSceneManagerPrivate::orientation(M::OrientationAngle angle) const
{
    return (angle == M::Angle0 || angle == M::Angle180) ? M::Landscape : M::Portrait;
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
    setParentItemForSceneWindow(sceneWindow, sceneWindow->windowType());

    sceneWindow->setZValue(zForWindowType(sceneWindow->windowType()));

    sceneWindow->hide();
}

void MSceneManagerPrivate::addSceneWindow(MSceneWindow *sceneWindow)
{
    Q_Q(MSceneManager);

    if (!sceneWindow->d_func()->sceneManager) {
        // Just add it
        addUnmanagedSceneWindow(sceneWindow);
        // This window was not receiving any events. Might not know about orientation change yet.
        MOrientationChangeEvent event(orientation(angle));
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

    // Give ownership of scene window to the caller.
    scene->removeItem(sceneWindow);
}

MSceneLayerEffect *MSceneManagerPrivate::createLayerEffectForWindow(MSceneWindow *window)
{
    MSceneLayerEffect *effect = 0;

    switch (window->windowType()) {
    case MSceneWindow::PopupList:
    case MSceneWindow::Dialog:
    case MSceneWindow::MessageBox:
    case MSceneWindow::ModalSceneWindow:
    case MSceneWindow::ApplicationMenu:
        effect = new MSceneLayerEffect("messagebox");
        break;
    case MSceneWindow::ObjectMenu:
        effect = new MSceneLayerEffect("objectmenu");
        break;
    default:
        return NULL;
    }

    //resize the effect layer
    setSceneWindowGeometry(effect);

    // Add effect to scene via rootElement
    setParentItemForSceneWindow(effect, window->windowType());

    effect->setZValue(zForWindowType(window->windowType()));

    // Add window as child of the effect
    window->setParentItem(effect);

    window->d_func()->effect = effect;

    return effect;
}

void MSceneManagerPrivate::setParentItemForSceneWindow(MSceneWindow *sceneWindow,
                                                       MSceneWindow::WindowType type)
{
    switch (type) {
        case MSceneWindow::EscapeButtonPanel:
        case MSceneWindow::NavigationBar:
        case MSceneWindow::ApplicationMenu:
            sceneWindow->setParentItem(navigationBarRootElement);
            break;
        case MSceneWindow::HomeButtonPanel:
        case MSceneWindow::NotificationInformation:
        case MSceneWindow::NotificationEvent:
            sceneWindow->setParentItem(homeButtonRootElement);
            break;
        default:
            sceneWindow->setParentItem(rootElement);
            break;
    }
}

void MSceneManagerPrivate::setSceneWindowGeometries()
{
    restoreSceneWindow();
    const int size = windows.size();
    MSceneWindow *sceneWindow;

    for (int i = 0; i < size; ++i) {
        sceneWindow = windows.at(i);
        if (sceneWindow->sceneWindowState() != MSceneWindow::Disappeared)
            setSceneWindowGeometry(sceneWindow);
    }
}

void MSceneManagerPrivate::setSceneWindowGeometry(MSceneWindow *window)
{
    if (window->isManagedManually())
        return;

    QPointF p = calculateSceneWindowPosition(window);
    window->setGeometry( QRectF(p, window->preferredSize()) );
}

void MSceneManagerPrivate::notifyWidgetsAboutOrientationChange()
{
    MOrientationChangeEvent event(orientation(angle));

    QList<QGraphicsItem *> sceneItems = scene->items();
    for(int i = sceneItems.count() - 1; i >= 0; i--) {
        QGraphicsItem *item = sceneItems.at(i);
        // event handlers might remove items from the scene
        // so we must check if item it's still there
        if (scene->items().contains(item))
            scene->sendEvent(item, &event);
    }
}

QPointF MSceneManagerPrivate::calculateSceneWindowPosition(MSceneWindow *window)
{
    Q_Q(MSceneManager);
    QSizeF sceneSize = q->visibleSceneSize(orientation(angle));
    QRectF availableSceneRect(QPointF(0,0), sceneSize);

    Qt::Alignment alignment = window->alignment();
    QSizeF windowSize = window->effectiveSizeHint(Qt::PreferredSize);

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

    return pos;
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
    Q_Q(MSceneManager);

    this->newAngle = newAngle;
    _q_changeGlobalOrientationAngle();

    QSize landscapeScreenSize = q->visibleSceneSize(M::Landscape);

    rootElement->setRotation(newAngle);
    if (orientation(angle) == M::Landscape) {
        rootElement->setPos(0, 0);
        rootElement->setTransformOriginPoint(landscapeScreenSize.width() / 2,
                                             landscapeScreenSize.height() / 2);
    } else {
        rootElement->setPos((landscapeScreenSize.width() - landscapeScreenSize.height()) / 2,
                            (landscapeScreenSize.height() - landscapeScreenSize.width()) / 2);
        rootElement->setTransformOriginPoint(landscapeScreenSize.height() / 2,
                                             landscapeScreenSize.width() / 2);
    }

    emit q->orientationChangeFinished(orientation(angle));
}

bool MSceneManagerPrivate::onApplicationPage(QGraphicsItem *item)
{
    QGraphicsItem *parent = item->parentItem();
    MApplicationPage *page = 0;

    if (parent) {
        do {
            page = dynamic_cast<MApplicationPage *>(parent);
            if (page)
                return true;
            parent = parent->parentItem();
        } while (parent != 0);
    }
    return false;
}

MSceneWindow *MSceneManagerPrivate::parentSceneWindow(QGraphicsItem *item)
{
    QGraphicsItem *parent = item->parentItem();
    MSceneWindow *parentSceneWindow = 0;

    if (parent) {
        do {
            parentSceneWindow = dynamic_cast<MSceneWindow *>(parent);
            if (parentSceneWindow)
                break;
            parent = parent->parentItem();
        } while (parent != 0);
    }

    return parentSceneWindow;
}

int MSceneManagerPrivate::scrollPageContents(MSceneWindow *window, int adjustment) const
{
    Q_Q(const MSceneManager);
    MApplicationPage *page = qobject_cast<MApplicationPage *>(window);

    if (!page) {
        // Nothing was scrolled.
        return 0;
    }

    if (!page->isPannable()) {
        // Scrolling was disabled explicitly.
        return 0;
    }

    // newAdjustment stores by how much the page's contents were scrolled.
    // It's > 0 when scrolling down, and < 0 when scrolling up.
    int newAdjustment(0);
    MPannableViewport *viewport(page->pannableViewport());

    // Need to find out whether there is enough scrollable contents for the requested amount:
    const int height = viewport->range().height() - viewport->position().y();

    if (height <= q->visibleSceneSize().height()) {
        // Nothing to do - page cannot be scrolled any further.
    } else if (adjustment >= 0) {
        newAdjustment = qMin(adjustment, height);
    } else if (-adjustment >= viewport->position().y()) {
        newAdjustment = -(viewport->position().y());
    } else { // qAbs(amount) \in [0, viewport->position()]
        newAdjustment = adjustment;
    }

    viewport->setPosition(QPoint(0, (newAdjustment + viewport->position().y())));
    // Disables kinetic scrolling until next pointer event - see NB #162913.
    viewport->physics()->stop();
    return newAdjustment;
}

void MSceneManagerPrivate::moveSceneWindow(MSceneWindow *window, int adjustment,
                                             int inputPanelHeight)
{
    if (!window) {
        return;
    }

    int newAdjustment(0);

    if (adjustment > 0) {
        newAdjustment = qMin(inputPanelHeight + sceneWindowTranslation.y(), adjustment);
    } else {
        const QPoint topLeftCorner = QPoint(0, 0);
        newAdjustment = qMax(static_cast<int>(window->mapToItem(rootElement, (topLeftCorner)).y()),
                             adjustment);
    }

    window->moveBy(0, -newAdjustment);
    sceneWindowTranslation.ry() -= newAdjustment;

    alteredSceneWindow = window;
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

void MSceneManagerPrivate::prepareWindowShow(MSceneWindow *window)
{
    Q_Q(MSceneManager);

    window->show();
    window->d_func()->dismissed = false;

    setSceneWindowGeometry(window);
    createLayerEffectForWindow(window);

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
        MPageSwitchAnimation::PageTransitionDirection direction)
{
    pageSwitchAnimation->setNewPage(newPage);
    pageSwitchAnimation->setOldPage(oldPage);
    pageSwitchAnimation->setPageTransitionDirection(direction);

    pageSwitchAnimation->start();
    freezeUIForAnimationDuration(pageSwitchAnimation);
}

void MSceneManagerPrivate::pushPage(MSceneWindow *page, bool animatedTransition)
{
    Q_Q(MSceneManager);
    MSceneWindow *previousPage = 0;

    if (currentPage && currentPage != page) {
        previousPage = currentPage;
        pageHistory.append(previousPage);
        emit q->pageHistoryChanged();
    }

    currentPage = page;

    if (animatedTransition) {
        if (previousPage)
            setSceneWindowState(previousPage, MSceneWindow::Disappearing);

        setSceneWindowState(currentPage, MSceneWindow::Appearing);

        startPageSwitchAnimation(currentPage, previousPage, MPageSwitchAnimation::RightToLeft);
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

    if (animatedTransition) {
        if (previousPage)
            setSceneWindowState(previousPage, MSceneWindow::Appearing);

        setSceneWindowState(currentPage, MSceneWindow::Disappearing);

        startPageSwitchAnimation(previousPage, currentPage, MPageSwitchAnimation::LeftToRight);
    } else {
        if (previousPage)
            setSceneWindowState(previousPage, MSceneWindow::Appeared);

        setSceneWindowState(currentPage, MSceneWindow::Disappeared);
    }

    currentPage = previousPage;
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
                if (pageSwitchAnimation->state() == QAbstractAnimation::Running) {
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
            if (sceneWindow->windowType() == MSceneWindow::ApplicationPage
                && pageSwitchAnimation->state() == QAbstractAnimation::Running) {
                foreach(MSceneWindowTransition transition, queuedTransitionsPageSwitchAnimation) {
                    if (transition.sceneWindow == sceneWindow
                        && transition.type == MSceneWindowTransition::DisappearTransition)
                    {
                        queuedTransitionsPageSwitchAnimation.removeAll(transition);
                        break;
                    }
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
            q->connect(window->d_func()->appearanceAnimation, SIGNAL(finished()),
                    SLOT(_q_onSceneWindowAppearanceAnimationFinished()));
            window->d_func()->appearanceAnimation->start();
        } else {
            setSceneWindowState(window, MSceneWindow::Appeared);
            if (window->windowType() == MSceneWindow::StatusBar) {
                qreal y = window->y() + window->geometry().height();
                navigationBarRootElement->setPos(0, y);
                homeButtonRootElement->setPos(0, y);
            }
        }
    }

}

void MSceneManagerPrivate::prepareWindowHide(MSceneWindow *window)
{
    Q_Q(MSceneManager);

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
                Q_ASSERT(pageSwitchAnimation->state() == QAbstractAnimation::Running);

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
                if (pageSwitchAnimation->state() == QAbstractAnimation::Running) {
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

        q->connect(window->d_func()->disappearanceAnimation, SIGNAL(finished()),
                SLOT(_q_onSceneWindowDisappearanceAnimationFinished()));

        window->d_func()->disappearanceAnimation->start();
    } else {
        setSceneWindowState(window, MSceneWindow::Disappeared);
        if (window->windowType() == MSceneWindow::StatusBar) {
            navigationBarRootElement->setPos(0, 0);
            homeButtonRootElement->setPos(0, 0);
        }
    }
}

void MSceneManagerPrivate::freezeUIForAnimationDuration(QAbstractAnimation *animation)
{
    Q_Q(MSceneManager);
    eventEater->show();

    if (animation->state() == QAbstractAnimation::Running) {
        QObject::connect(animation, SIGNAL(finished()), q, SLOT(_q_unFreezeUI()));
    }
}

void MSceneManagerPrivate::createAppearanceAnimationForSceneWindow(MSceneWindow *sceneWindow)
{
    Q_ASSERT(sceneWindow->d_func()->appearanceAnimation == 0);
    MAbstractWidgetAnimation *animation;

    // TODO: Get this from theme/CSS
    switch(sceneWindow->windowType()) {
        case MSceneWindow::Dialog:
        case MSceneWindow::NotificationInformation:
        case MSceneWindow::NotificationEvent:
        case MSceneWindow::ApplicationMenu:
        case MSceneWindow::NavigationBar:
        case MSceneWindow::PopupList: {
            MWidgetSlideAnimation *slideInAnimation = new MWidgetSlideAnimation(sceneWindow);
            slideInAnimation->setTransitionDirection(MWidgetSlideAnimation::In);
            animation = slideInAnimation;
            break;
        }
        case MSceneWindow::MessageBox: {
            MWidgetZoomAnimation *objectMenuAnimation =
                    new MWidgetZoomAnimation(sceneWindow);

            objectMenuAnimation->setOrigin(sceneWindow->boundingRect().center());
            objectMenuAnimation->setTransitionDirection(MWidgetZoomAnimation::In);

            animation = objectMenuAnimation;
            break;
        }
        case MSceneWindow::StatusBar: {
            MWidgetSlideAnimation *slideInAnimation = new MWidgetSlideAnimation(sceneWindow);
            slideInAnimation->setTransitionDirection(MWidgetSlideAnimation::In);
            animation = slideInAnimation;

            QList<QGraphicsWidget*> list;
            list << navigationBarRootElement << homeButtonRootElement;
            foreach(QGraphicsWidget *widget, list) {
                MWidgetMoveAnimation *moveAnimation = new MWidgetMoveAnimation;
                moveAnimation->setWidget(widget);
                moveAnimation->setFinalPos(QPointF(widget->x(), widget->y() + sceneWindow->effectiveSizeHint(Qt::PreferredSize).height()));
                animation->addAnimation(moveAnimation);
            }
            break;
        }
        case MSceneWindow::ObjectMenu: {
            MWidgetZoomAnimation *objectMenuAnimation =
                    new MWidgetZoomAnimation(sceneWindow);

            MObjectMenu *objectMenu = static_cast<MObjectMenu*>(sceneWindow);
            objectMenuAnimation->setOrigin(rootElement->mapFromScene(objectMenu->cursorPosition()));
            objectMenuAnimation->setTransitionDirection(MWidgetZoomAnimation::In);
            animation = objectMenuAnimation;
            break;
        }
        default: {
            MWidgetFadeAnimation *fadeInAnimation = new MWidgetFadeAnimation(sceneWindow);
            fadeInAnimation->setTransitionDirection(MWidgetFadeAnimation::In);
            animation = fadeInAnimation;
            break;
        }
    }

    animation->setTargetWidget(sceneWindow);

    MSceneWindow *effect = sceneWindow->d_func()->effect;
    if (effect)
        animation->addAnimation(effect->d_func()->appearanceAnimation);

    sceneWindow->d_func()->appearanceAnimation = animation;
}

void MSceneManagerPrivate::createDisappearanceAnimationForSceneWindow(MSceneWindow *sceneWindow)
{
    Q_ASSERT(sceneWindow->d_func()->disappearanceAnimation == 0);
    MAbstractWidgetAnimation *animation;

    // TODO: Get this from theme/CSS
    switch(sceneWindow->windowType()) {
        case MSceneWindow::Dialog:
        case MSceneWindow::NotificationInformation:
        case MSceneWindow::NotificationEvent:
        case MSceneWindow::ApplicationMenu:
        case MSceneWindow::NavigationBar:
        case MSceneWindow::PopupList: {
            MWidgetSlideAnimation *slideOutAnimation = new MWidgetSlideAnimation(sceneWindow);
            slideOutAnimation->setTransitionDirection(MWidgetSlideAnimation::Out);
            animation = slideOutAnimation;
            break;
        }
        case MSceneWindow::MessageBox: {
            MWidgetZoomAnimation *zoomAnimation =
                    new MWidgetZoomAnimation(sceneWindow);

            zoomAnimation->setOrigin(sceneWindow->boundingRect().center());
            zoomAnimation->setTransitionDirection(MWidgetZoomAnimation::Out);

            animation = zoomAnimation;
            break;
        }
        case MSceneWindow::StatusBar: {
            MWidgetSlideAnimation *slideOutAnimation = new MWidgetSlideAnimation(sceneWindow);
            slideOutAnimation->setTransitionDirection(MWidgetSlideAnimation::Out);
            animation = slideOutAnimation;

            QList<QGraphicsWidget*> list;
            list << navigationBarRootElement << homeButtonRootElement;
            foreach(QGraphicsWidget *widget, list) {
                MWidgetMoveAnimation *moveAnimation = new MWidgetMoveAnimation;
                moveAnimation->setWidget(widget);
                moveAnimation->setFinalPos(QPointF(widget->x(), widget->y() - sceneWindow->effectiveSizeHint(Qt::PreferredSize).height()));
                animation->addAnimation(moveAnimation);
            }
            break;
        }
        case MSceneWindow::ObjectMenu: {
            MWidgetZoomAnimation *zoomAnimation =
                    new MWidgetZoomAnimation(sceneWindow);

            MObjectMenu *objectMenu = static_cast<MObjectMenu*>(sceneWindow);
            zoomAnimation->setOrigin(rootElement->mapFromScene(objectMenu->cursorPosition()));
            zoomAnimation->setTransitionDirection(MWidgetZoomAnimation::Out);

            animation = zoomAnimation;
            break;
        }
        default: {
            MWidgetFadeAnimation *fadeOutAnimation = new MWidgetFadeAnimation(sceneWindow);
            fadeOutAnimation->setTransitionDirection(MWidgetFadeAnimation::Out);
            animation = fadeOutAnimation;
            break;
        }
    }

    animation->setTargetWidget(sceneWindow);

    MSceneWindow *effect = sceneWindow->d_func()->effect;
    if (effect)
        animation->addAnimation(effect->d_func()->disappearanceAnimation);

    sceneWindow->d_func()->disappearanceAnimation = animation;
}

void MSceneManagerPrivate::setSceneWindowState(MSceneWindow *sceneWindow,
        MSceneWindow::SceneWindowState newState)
{
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
}

void MSceneManagerPrivate::onSceneWindowEnteringAppearingState(MSceneWindow *sceneWindow)
{
    prepareWindowShow(sceneWindow);
    produceSceneWindowEvent(MSceneWindowEvent::eventTypeAppear(), sceneWindow, true);
}

void MSceneManagerPrivate::onSceneWindowEnteringAppearedState(MSceneWindow *sceneWindow)
{
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

    if (isOnDisplay()) {
        produceMustBeResolvedDisplayEvent(sceneWindow);
    }
}

void MSceneManagerPrivate::onSceneWindowEnteringDisappearingState(MSceneWindow *sceneWindow)
{
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

                if (isOnDisplay()) {
                    produceFullyOffDisplayEvents(sceneWindow);
                }

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
            produceSceneWindowEvent(MSceneWindowEvent::eventTypeDisappear(),
                    sceneWindow, false);
            prepareWindowHide(sceneWindow);

            if (isOnDisplay()) {
                produceFullyOffDisplayEvents(sceneWindow);
            }
            break;

        default:
            break;
    }

    if (currentPage == sceneWindow)
        currentPage = 0;

    if (sceneWindow->windowType() == MSceneWindow::StatusBar)
        statusBar = 0;

    // If there is a layer effect it is deleted
    if (sceneWindow->d_func()->effect) {
        sceneWindow->setParentItem(0);
        delete sceneWindow->d_func()->effect;
        sceneWindow->d_func()->effect = 0;
    }

    sceneWindow->hide();

    if (sceneWindow->sceneWindowState() == MSceneWindow::Disappearing ||
            sceneWindow->sceneWindowState() == MSceneWindow::Appeared) {
        if ((sceneWindow->deletionPolicy() == MSceneWindow::DestroyWhenDone) ||
                ((sceneWindow->deletionPolicy() == MSceneWindow::DestroyWhenDismissed)
                 && sceneWindow->d_func()->dismissed)) {
            sceneWindow->deleteLater();
        }
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

    bool animatedTransition = d->isOnDisplay();

    d->appearSceneWindow(window, policy, animatedTransition);
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

    bool animatedTransition = d->isOnDisplay();

    window->d_func()->dismissed = false;
    d->disappearSceneWindow(window, animatedTransition);
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *window)
{
    Q_D(MSceneManager);
    window->d_func()->dismissed = false;
    d->disappearSceneWindow(window, false);
}

void MSceneManager::dismissSceneWindow(MSceneWindow *window)
{
    Q_D(MSceneManager);

    bool animatedTransition = d->isOnDisplay();

    window->d_func()->dismissed = true;
    d->disappearSceneWindow(window, animatedTransition);
}

void MSceneManager::dismissSceneWindowNow(MSceneWindow *window)
{
    Q_D(MSceneManager);
    window->d_func()->dismissed = true;
    d->disappearSceneWindow(window, false);
}

bool MSceneManager::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);

    event->ignore();
    return false;
}

void MSceneManager::requestSoftwareInputPanel(QGraphicsWidget *inputWidget)
{
    Q_D(MSceneManager);

    if (inputWidget) {
        d->focusedInputWidget = inputWidget;
        QInputContext *inputContext = qApp->inputContext();

        if (!inputContext) {
            return;
        }

        QWidget *focusWidget = QApplication::focusWidget();

        if (focusWidget) {
            // FIXME: this is a temporary workaround because of the
            // QGraphicsView unable to correctly update the attribute.
            // We're waiting for fixing this on Qt side.
            focusWidget->setAttribute(Qt::WA_InputMethodEnabled, true);
            //enforce update if focus is moved from one MTextEdit to other
            //if attribute WA_InputMethodEnabled is not set then Qt will call
            //setFocusWidget automatically
            inputContext->setFocusWidget(focusWidget);
        }

        //FIXME: verify if application style allows SIP usage
        QEvent request(QEvent::RequestSoftwareInputPanel);
        inputContext->filterEvent(&request);

        // This is normally called automatically except in cases where input panel area does not change and we
        // move between two similar text edits, for example.
        d->relocateWindowByInputPanel(MInputMethodState::instance()->inputMethodArea());
    }
}

void MSceneManager::closeSoftwareInputPanel()
{
    Q_D(MSceneManager);

    QInputContext *inputContext = qApp->inputContext();

    // Tell input context we want to close the SIP.
    if (!inputContext) {
        return;
    }

    //FIXME: verify if application style allows SIP usage
    QEvent close(QEvent::CloseSoftwareInputPanel);
    inputContext->filterEvent(&close);
    inputContext->reset();

    d->focusedInputWidget = 0;
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
        if (mode == AnimatedTransition) {
            bool managesVisibleWindow = false;

            // Only animate the rotation if it is actually visible to the user
            QList<QGraphicsView *> viewsList = scene()->views();
            for (int i = 0; i < viewsList.count(); ++i) {
                MWindow *window = qobject_cast<MWindow *>(viewsList[i]);

                if (window && window->isOnDisplay()) {
                    if (window->windowState() != Qt::WindowMinimized) {
                        managesVisibleWindow = true;
                    }
                    break;
                }
            }
            if (managesVisibleWindow) {
                d->rotateToAngle(angle);
            } else {
                d->setOrientationAngleWithoutAnimation(angle);
            }
        } else {
            d->setOrientationAngleWithoutAnimation(angle);
        }
    }
}

M::Orientation MSceneManager::orientation() const
{
    Q_D(const MSceneManager);

    return d->orientation(d->angle);
}

M::OrientationAngle MSceneManager::orientationAngle() const
{
    Q_D(const MSceneManager);

    return d->angle;
}

QSize MSceneManager::visibleSceneSize(M::Orientation orientation) const
{
    QSize s;

    if (orientation == M::Landscape) {
        s = MDeviceProfile::instance()->resolution();
    } else {
        s = QSize(MDeviceProfile::instance()->resolution().height(),
                  MDeviceProfile::instance()->resolution().width());
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
    Q_D(MSceneManager);

    if (event->added() && event->child()->objectName() == "_m_testBridge") {
        d->debugInterface = event->child();
        new MSceneManagerTestInterface(d, d->debugInterface);
    } else if (event->child()->objectName() == "_m_testBridge") {
        d->debugInterface = 0;
    }
}

MSceneManagerTestInterface::MSceneManagerTestInterface(
        MSceneManagerPrivate *d, QObject *parent)
    : QObject(parent), d(d)
{
}

void MSceneManagerTestInterface::fastForwardPageSwitchAnimation()
{
    d->pageSwitchAnimation->setCurrentTime(d->pageSwitchAnimation->duration());
}

void MSceneManagerTestInterface::fastForwardSceneWindowTransitionAnimation(MSceneWindow *sceneWindow)
{
    d->fastForwardSceneWindowTransitionAnimation(sceneWindow);
}

void MSceneManagerTestInterface::addSceneWindow(MSceneWindow *sceneWindow)
{
    d->addSceneWindow(sceneWindow);
}

void MSceneManagerTestInterface::removeSceneWindow(MSceneWindow *sceneWindow)
{
    d->removeSceneWindow(sceneWindow);
}

#include "moc_mscenemanager.cpp"
