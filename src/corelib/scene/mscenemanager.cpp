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
#include "mescapebuttonpanel.h"
#include "mescapebuttonpanelmodel.h"
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
#include "mabstractwidgetanimation.h"
#include "mpageswitchanimation.h"

#include <mwidgetslideinanimation.h>
#include <mwidgetfadeinanimation.h>
#include <mwidgetslideoutanimation.h>
#include <mwidgetfadeoutanimation.h>

namespace
{
    //! This is a guess for the SIP close timeout since there is no general way of getting it.
    const int SoftwareInputPanelHideTimer = 500;
}


void MSceneManagerPrivate::init(MScene *scene)
{
    Q_Q(MSceneManager);

    this->scene = scene;

    navBar = 0;
    escapeButtonPanel = 0;
    currentPage = 0;
    navBarHidden = false;
    escapeButtonHidden = false;

    focusedInputWidget = 0;
    alteredSceneWindow = 0;
    sceneWindowTranslation = QPoint();

    pendingSIPClose = false;

    statusBar = 0;

    pendingRotation = 0;

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

    windows = new QList<MSceneWindow *>();

    rootElement = new QGraphicsWidget();
    rootElement->setTransformOriginPoint(QPointF(q->visibleSceneSize().width() / 2.0, q->visibleSceneSize().height() / 2.0));
    scene->addItem(rootElement);

    //TODO: get this from theme
    orientationAnimation = new MBasicOrientationAnimation(q->visibleSceneSize(M::Landscape));
    orientationAnimation->setRootElement(rootElement);
    q->connect(orientationAnimation, SIGNAL(orientationChanged()), SLOT(_q_changeGlobalOrientationAngle()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_emitOrientationChangeFinished()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_applyQueuedSceneWindowTransitions()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_triggerAsyncPendingOrientationChange()));

    // The scene manager should only listen to region updates from one instance, to prevent
    // conflicting window relocation requests. Since MIMS is a singleton, enforcing
    // Qt::UniqueConnection is sufficient.
    // This also implies that the software input panel could come up without an explicit request
    // from the application side.
    q->connect(MInputMethodState::instance(), SIGNAL(inputMethodAreaChanged(QRect)),
               q, SLOT(_q_relocateWindowByInputPanel(QRect)),
               Qt::UniqueConnection);

    pageSwitchAnimation = new MPageSwitchAnimation;

    setOrientationAngleWithoutAnimation(newAngle);
}

MSceneManagerPrivate::~MSceneManagerPrivate()
{
    delete windows;
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

    rootElement->setEnabled(true);

    QAbstractAnimation *animation = dynamic_cast<QAbstractAnimation *>(q->sender());
    if (animation == 0)
        return;

    QObject::disconnect(animation, SIGNAL(finished()), q, SLOT(_q_unFreezeUI()));
}

void MSceneManagerPrivate::_q_applyQueuedSceneWindowTransitions()
{
    SceneWindowTransition transition;

    while (queuedTransitions.count() > 0) {
        transition = queuedTransitions.takeFirst();

        switch (transition.type) {
            case SceneWindowTransition::AppearTransition:
                appearSceneWindow(transition.sceneWindow, transition.policy, transition.animated);
                break;
            case SceneWindowTransition::DisappearTransition:
                disappearSceneWindow(transition.sceneWindow, transition.animated);
                break;
            case SceneWindowTransition::DismissTransition:
                dismissSceneWindow(transition.sceneWindow, transition.animated);
                break;
            default:
                // Should never occur.
                qFatal("MSceneManager: Invalid SceneWindowTransition::TransitionType value.");
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

void MSceneManagerPrivate::_q_onSceneWindowAppeared()
{
    Q_Q(MSceneManager);
    MSceneWindow *window = dynamic_cast<MSceneWindow *>(q->sender());
    if (window == 0)
        return;

    if (isOnDisplay()) {
        produceMustBeResolvedDisplayEvent(window);
    }

    QObject::disconnect(window, SIGNAL(appeared()), q, SLOT(_q_onSceneWindowAppeared()));
}

void MSceneManagerPrivate::_q_onSceneWindowDisappeared()
{
    Q_Q(MSceneManager);
    //TODO: There could be a mapping between windows and animations so that
    //      when a window is detached all its animations are deleted as well
    //      so this method would never got called for "invalid"/detached window.
    MSceneWindow *window = dynamic_cast<MSceneWindow *>(q->sender());
    if (window == 0)
        return;

    if (window->windowType() == MSceneWindow::NavigationBar && !navBarHidden)
        navBar = 0;

    if (window->windowType() == MSceneWindow::EscapeButtonPanel && !escapeButtonHidden)
        escapeButtonPanel = 0;

    if (window->windowType() == MSceneWindow::StatusBar) {
        statusBar = 0;

        // All other scene windows have to be repositioned since now there's more
        // space available in the scene.
        setSceneWindowGeometries();
    }

    if (isOnDisplay()) {
        produceFullyOffDisplayEvents(window);
    }

    window->hide();

    if (window->d_func()->disappearanceAnimation)
        window->d_func()->disappearanceAnimation->restoreTargetWidgetState();

    QObject::disconnect(window, SIGNAL(disappeared()), q, SLOT(_q_onSceneWindowDisappeared()));

    //check if the window has not yet been detached
    int i = windows->indexOf(window);

    if (i != -1) {
        windows->removeAt(i);
        QObject::disconnect(window, SIGNAL(repositionNeeded()), q, SLOT(_q_setSenderGeometry()));

        // If there is a layer effect it is deleted as well
        if (window->d_func()->effect) {
            window->setParentItem(0);
            delete window->d_func()->effect;
            window->d_func()->effect = 0;
        }

        if ((window->deletionPolicy() == MSceneWindow::DestroyWhenDone) ||
                ((window->deletionPolicy() == MSceneWindow::DestroyWhenDismissed)
                 && window->d_func()->dismissed)) {
            window->deleteLater();
        }
    }
}

void MSceneManagerPrivate::_q_relocateWindowByInputPanel(const QRect &inputPanelRect)
{
    Q_Q(MSceneManager);

    const QRectF microFocusRect = q->scene()->inputMethodQuery(Qt::ImMicroFocus).toRectF();
    // If the microfocus is invalid then the focused widget does not want to be relocated.
    if (!microFocusRect.isValid()) {
        return;
    }

    // This method is not responsible for restoring visibility when the input panel is closed -
    // _q_inputPanelClosed() does that. Therefore, it is OK to also ignore empty rectangles here.
    if (!focusedInputWidget || inputPanelRect.isEmpty()) {
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

    // Find the first scene window parent of the focused input widget, since only those should
    // be scrolled or moved in this context. The altered scene window is moved back to its
    // original location, although - if it has pannable contents - it is not scrolled back.
    MSceneWindow *newParent = parentSceneWindow(focusedInputWidget);
    adjustment -= scrollPageContents(newParent, adjustment);
    moveSceneWindow(newParent, adjustment, obstructedHeight);
}

void MSceneManagerPrivate::_q_restoreSceneWindow()
{
    if (alteredSceneWindow && !focusedInputWidget) {
        sceneWindowTranslation *= -1;
        alteredSceneWindow->moveBy(sceneWindowTranslation.x(), sceneWindowTranslation.y());
        sceneWindowTranslation = QPoint();
        alteredSceneWindow = 0;
    }
}

void MSceneManager::ensureCursorVisible()
{
    Q_D(MSceneManager);

    d->_q_relocateWindowByInputPanel(MInputMethodState::instance()->inputMethodArea());
}

M::Orientation MSceneManagerPrivate::orientation(M::OrientationAngle angle) const
{
    return (angle == M::Angle0 || angle == M::Angle180) ? M::Landscape : M::Portrait;
}

void MSceneManagerPrivate::attachWindow(MSceneWindow *window)
{
    if (!windows->contains(window))
        windows->append(window);
}

void MSceneManagerPrivate::detachWindow(MSceneWindow *window)
{
    windows->removeOne(window);

    // If there is a layer effect it is deleted as well
    if (window->d_func()->effect) {
        window->setParentItem(rootElement);
        delete window->d_func()->effect;
        window->d_func()->effect = 0;
    }
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
    effect->setParentItem(rootElement);
    effect->setZValue(zForWindowType(window->windowType()));

    // window could have been added to another scene manually beforehand
    // remove it in that case, to avoid Qt's assert
    if (window->scene() && window->scene() != scene)
        window->scene()->removeItem(window);

    // Add window as child of the effect
    window->setParentItem(effect);

    window->d_func()->effect = effect;

    return effect;
}

void MSceneManagerPrivate::setSceneWindowGeometries()
{
    _q_restoreSceneWindow();
    const int size = windows->size();
    for (int i = 0; i < size; ++i)
        setSceneWindowGeometry(windows->at(i));
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

QRectF MSceneManagerPrivate::calculateAvailableSceneRect(MSceneWindow *window)
{
    Q_Q(MSceneManager);

    QSizeF sceneSize = q->visibleSceneSize(orientation(angle));
    QRectF availableSceneRect(QPointF(0,0), sceneSize);

    if (statusBar) {
        const MSceneWindow::WindowType type = window->windowType();
        if( type != MSceneWindow::StatusBar && type != MSceneWindow::LayerEffect) {
            QSizeF windowSize = window->effectiveSizeHint(Qt::PreferredSize);

            qreal statusBarBottomEdge = statusBar->y() +
                statusBar->effectiveSizeHint(Qt::PreferredSize).height();

            if ((statusBarBottomEdge + windowSize.height()) < sceneSize.height()) {
                // Window should still fit within the remaining scene space when shifted
                // downwards due to the status bar presence
                availableSceneRect.setY(statusBarBottomEdge);

                Q_ASSERT(availableSceneRect.height() > 0.0);
            }
        }
    }

    return availableSceneRect;
}

QPointF MSceneManagerPrivate::calculateSceneWindowPosition(MSceneWindow *window)
{
    // The rectangle available for the positioning of the given scene window
    QRectF availableSceneRect = calculateAvailableSceneRect(window);

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

bool MSceneManagerPrivate::validateSceneWindowPreAppearanceStatus(MSceneWindow *sceneWindow)
{
    Q_Q(MSceneManager);
    bool statusOk = true;

    if (sceneWindow->d_func()->shown) {
        MSceneManager *otherSceneManager = sceneWindow->sceneManager();
        Q_ASSERT(otherSceneManager != 0);

        if (otherSceneManager != q) {
            // Cannot be in two scenes simultaneously.

            MSceneWindow::DeletionPolicy currentPolicy = sceneWindow->deletionPolicy();

            otherSceneManager->disappearSceneWindowNow(sceneWindow);

            if (currentPolicy == MSceneWindow::DestroyWhenDone) {
                // Window is gone.
                statusOk = false;
            }
        } else {
            // Window is already being shown here.
            statusOk = false;
        }
    }

    return statusOk;
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

    q->scene()->sendEvent(sceneWindow, &displayEvent);
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
            q->scene()->sendEvent(mWidget, &event);
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

    attachWindow(window);

    // Check whether we are trying to show a window while it is in the middle of
    // a hide animation. If that's the case, we stop it.
    if (window->d_func()->disappearanceAnimation) {
        MAbstractWidgetAnimation *disappearanceAnimation = window->d_func()->disappearanceAnimation;

        if (disappearanceAnimation->state() == QAbstractAnimation::Running) {
            disappearanceAnimation->stop();
            QObject::disconnect(window, SIGNAL(disappeared()),
                    q, SLOT(_q_onSceneWindowDisappeared()));

            disappearanceAnimation->restoreTargetWidgetState();
        }
    }

    setSceneWindowGeometry(window);
    MSceneLayerEffect *effect = createLayerEffectForWindow(window);
    if (effect) {
        effect->enableEffect();
    } else {
        // window could have been added to another scene manually beforehand
        // remove it in that case, to avoid Qt's assert
        if (window->scene() && window->scene() != scene)
            window->scene()->removeItem(window);

        //add window to scene if not already there
        if (scene->items().indexOf(window) == -1) {
            window->setParentItem(rootElement);
            window->setZValue(zForWindowType(window->windowType()));
        }
    }

    if (window->windowType() == MSceneWindow::NavigationBar)
        navBar = qobject_cast<MNavigationBar *>(window);

    if (window->windowType() == MSceneWindow::EscapeButtonPanel)
        escapeButtonPanel = window;

    if (window->windowType() == MSceneWindow::StatusBar) {
        // There can be only one status bar in the scene.
        Q_ASSERT(statusBar == 0);

        statusBar = window;

        // All other scene windows have to be repositioned since now there's less
        // space available in the scene for them.
        setSceneWindowGeometries();
    }

    window->show();
    window->d_func()->shown = true;

    window->d_func()->dismissed = false;

    orientationAnimation->addSceneWindow(window);

    q->connect(window, SIGNAL(appeared()), SLOT(_q_onSceneWindowAppeared()));
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
        prepareWindowHide(currentPage);
        produceSceneWindowEvent(MSceneWindowEvent::eventTypeDisappear(), currentPage,
                animatedTransition);

        previousPage = currentPage;
        pageHistory.append(previousPage);
        emit q->pageHistoryChanged();
    }

    currentPage = page;

    if (animatedTransition) {
        startPageSwitchAnimation(currentPage, previousPage, MPageSwitchAnimation::LeftToRight);
    } else {
        if (previousPage) {
            emit previousPage->disappeared();
        }
        emit currentPage->appeared();
    }
}

void MSceneManagerPrivate::popPage(bool animatedTransition)
{
    Q_Q(MSceneManager);
    MSceneWindow *previousPage = 0;

    // Pages in the history might have been deleted overtime.
    while (previousPage == 0 && !pageHistory.isEmpty()) {
        previousPage = pageHistory.takeLast();
        emit q->pageHistoryChanged();
    }

    if (previousPage) {
        prepareWindowShow(previousPage);
        produceSceneWindowEvent(MSceneWindowEvent::eventTypeAppear(), previousPage,
                animatedTransition);
    }

    if (animatedTransition) {
        startPageSwitchAnimation(previousPage, currentPage, MPageSwitchAnimation::RightToLeft);
    } else {
        emit currentPage->disappeared();

        if (previousPage) {
            emit previousPage->appeared();
        }
    }

    currentPage = previousPage;
}

void MSceneManagerPrivate::appearSceneWindow(MSceneWindow *window,
        MSceneWindow::DeletionPolicy policy,
        bool animatedTransition)
{
    if (debugInterface) {
        if (animatedTransition)
            debugInterface->setProperty("transitionMode", MSceneManager::AnimatedTransition);
        else
            debugInterface->setProperty("transitionMode", MSceneManager::ImmediateTransition);
    }


    // Popping up scene windows during an orientation change is
    // a grey area. We probably want to avoid them during that period.
    // TODO: For now we are only queueing the status bar. We should
    // think about out a policy to apply to all scene windows.
    if (orientationAnimation->state() == QAbstractAnimation::Running
            && window->windowType() == MSceneWindow::StatusBar) {

        SceneWindowTransition transition;
        transition.sceneWindow = window;
        transition.type = SceneWindowTransition::AppearTransition;
        transition.policy = policy;
        transition.animated = animatedTransition;
        queuedTransitions.append(transition);

        return;
    }

    if (!validateSceneWindowPreAppearanceStatus(window)) {
        return;
    }

    Q_ASSERT(window->d_func()->shown == false);

    produceSceneWindowEvent(MSceneWindowEvent::eventTypeAppear(), window,
                            animatedTransition);
    prepareWindowShow(window);

    window->d_func()->policy = policy;

    if (window->windowType() == MSceneWindow::ApplicationPage) {
        pushPage(window, animatedTransition);
    } else {
        if (animatedTransition) {
            if (!window->d_func()->appearanceAnimation)
                createAppearanceAnimationForSceneWindow(window);
            window->d_func()->appearanceAnimation->start();
        } else {
            emit window->appeared();
        }
    }

}

void MSceneManagerPrivate::prepareWindowHide(MSceneWindow *window)
{
    Q_Q(MSceneManager);

    QObject::disconnect(window, SIGNAL(repositionNeeded()),
                        q, SLOT(_q_setSenderGeometry()));
    orientationAnimation->removeSceneWindow(window);
    window->d_func()->shown = false;

    if (window->d_func()->effect) {
        window->d_func()->effect->disableEffect();
    }

    // Check whether we are trying to hide a window while it is in the middle of
    // a show animation. If that's the case, we stop it.
    if (window->d_func()->appearanceAnimation) {
        MAbstractWidgetAnimation *appearanceAnimation = window->d_func()->appearanceAnimation;

        if (appearanceAnimation->state() == QAbstractAnimation::Running) {
            appearanceAnimation->stop();
            QObject::disconnect(window, SIGNAL(appeared()),
                    q, SLOT(_q_onSceneWindowAppeared()));

            appearanceAnimation->restoreTargetWidgetState();
        }
    }

    q->connect(window, SIGNAL(disappeared()), SLOT(_q_onSceneWindowDisappeared()));
}

void MSceneManagerPrivate::disappearSceneWindow(MSceneWindow *window,
        bool animatedTransition)
{
    // Disappearing scene windows during an orientation change is
    // a grey area. We probably want to avoid them during that period.
    // TODO: For now we are only queueing the status bar. We should
    // think about out a policy to apply to all scene windows.
    if (orientationAnimation->state() == QAbstractAnimation::Running
            && window->windowType() == MSceneWindow::StatusBar) {

        SceneWindowTransition transition;
        transition.sceneWindow = window;
        transition.type = SceneWindowTransition::DisappearTransition;
        transition.animated = animatedTransition;
        queuedTransitions.append(transition);

        return;
    }

    produceSceneWindowEvent(MSceneWindowEvent::eventTypeDisappear(), window,
                            animatedTransition);
    prepareWindowHide(window);

    if (window->windowType() == MSceneWindow::ApplicationPage) {
        if (window == currentPage) {
            currentPage = 0;
        }
    }

    if (animatedTransition) {
        if (!window->d_func()->disappearanceAnimation)
            createDisappearanceAnimationForSceneWindow(window);
        window->d_func()->disappearanceAnimation->start();
    } else {
        emit window->disappeared();
    }
}

void MSceneManagerPrivate::freezeUIForAnimationDuration(QAbstractAnimation *animation)
{
    Q_Q(MSceneManager);

    if (animation->state() == QAbstractAnimation::Running) {
        //rootElement->setEnabled(false);
        QObject::connect(animation, SIGNAL(finished()), q, SLOT(_q_unFreezeUI()));
    }
}

void MSceneManagerPrivate::dismissSceneWindow(MSceneWindow *window,
        bool animatedTransition)
{
    // Dismissing scene windows during an orientation change is
    // a grey area. We probably want to avoid them during that period.
    // TODO: For now we are only queueing the status bar. We should
    // think about out a policy to apply to all scene windows.
    if (orientationAnimation->state() == QAbstractAnimation::Running
            && window->windowType() == MSceneWindow::StatusBar) {

        SceneWindowTransition transition;
        transition.sceneWindow = window;
        transition.type = SceneWindowTransition::DismissTransition;
        transition.animated = animatedTransition;
        queuedTransitions.append(transition);

        return;
    }

    produceSceneWindowEvent(MSceneWindowEvent::eventTypeDismiss(), window,
                            animatedTransition);
    prepareWindowHide(window);

    window->d_func()->dismissed = true;

    if (window->windowType() == MSceneWindow::ApplicationPage) {
        if (window == currentPage) {
            popPage(animatedTransition);
        }
        // If the window is not currentPage then it means that it's not
        // currently being displayed and thus there's nothing to be done.

    } else if (animatedTransition) { // Fallback to legacy disappearance anim.
        if (!window->d_func()->disappearanceAnimation)
            createDisappearanceAnimationForSceneWindow(window);
        window->d_func()->disappearanceAnimation->start();
    } else {
        emit window->disappeared();
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
        case MSceneWindow::PopupList:
            animation = new MWidgetSlideInAnimation(sceneWindow);
            break;
        default:
            animation = new MWidgetFadeInAnimation(sceneWindow);
            break;
    }

    animation->setTargetWidget(sceneWindow);
    sceneWindow->connect(animation, SIGNAL(finished()), SIGNAL(appeared()));
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
        case MSceneWindow::PopupList:
            animation = new MWidgetSlideOutAnimation(sceneWindow);
            break;
        default:
            animation = new MWidgetFadeOutAnimation(sceneWindow);
            break;
    }

    animation->setTargetWidget(sceneWindow);
    sceneWindow->connect(animation, SIGNAL(finished()), SIGNAL(disappeared()));
    sceneWindow->d_func()->disappearanceAnimation = animation;
}

void MSceneManagerPrivate::_q_inputPanelOpened()
{
    Q_Q(MSceneManager);

    if (!focusedInputWidget)
        return;

    const bool widgetOnPage = onApplicationPage(focusedInputWidget);
    if (navBar && widgetOnPage) {
        navBarHidden = true;
        q->disappearSceneWindow(navBar);
    }
    if (escapeButtonPanel && (widgetOnPage || (navBar && navBar->isAncestorOf(focusedInputWidget)))) {
        escapeButtonHidden = true;
        q->disappearSceneWindow(escapeButtonPanel);
    }

    // Have to call the slot explicitly at least once: The focused widget can change without the
    // MIMS sending new signals (e.g., the input field's content type stays the same).
    // TODO: Rethink the design of MIMS to allow pending updates (so that we'll *know* wether we
    // get a signal later or not).
    _q_relocateWindowByInputPanel(MInputMethodState::instance()->inputMethodArea());
}

void MSceneManagerPrivate::_q_inputPanelClosed()
{
    Q_Q(MSceneManager);

    if (!pendingSIPClose) {
        return;
    }

    focusedInputWidget = 0;

    if (navBar && navBarHidden) {
        q->appearSceneWindow(navBar);
        navBarHidden = false;
    }
    if (escapeButtonPanel && escapeButtonHidden) {
        q->appearSceneWindow(escapeButtonPanel);
        escapeButtonHidden = false;
    }

    _q_restoreSceneWindow();
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

    bool animatedTransition = d->isOnDisplay();

    d->appearSceneWindow(window, policy, animatedTransition);
}

void MSceneManager::appearSceneWindowNow(MSceneWindow *window, MSceneWindow::DeletionPolicy policy)
{
    Q_D(MSceneManager);
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

    d->disappearSceneWindow(window, animatedTransition);
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *window)
{
    Q_D(MSceneManager);
    d->disappearSceneWindow(window, false);
}

void MSceneManager::dismissSceneWindow(MSceneWindow *window)
{
    Q_D(MSceneManager);

    bool animatedTransition = d->isOnDisplay();

    d->dismissSceneWindow(window, animatedTransition);
}

void MSceneManager::dismissSceneWindowNow(MSceneWindow *window)
{
    Q_D(MSceneManager);
    d->dismissSceneWindow(window, false);
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

        // Calling _q_relocateWindowByInputPanel can change the scene window position and it is
        // common that we got here with mousePressEvent in the backtrace. Moving the
        // scene window now can cause mouse press to not hit its target (which causes focus lost).
        // Because of this we shall visit event loop first.
        // EDIT: This should not happen anymore with commit "cf02401 Changes: In MTextEdit,
        // move the SIP request to the mouse-release event".
        d->pendingSIPClose = false;
        QTimer::singleShot(0, this, SLOT(_q_inputPanelOpened()));

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

    // There is no general way of tracking when the SIP actually closes. It might not
    // close at all, for example, in a situation where focus is changed between widgets
    // that both wants a SIP. MInputContext has its own logic for this because it cannot
    // rely that it's running on a MApplication.
    // Our guess now is that SIP will be closed after some delay.
    d->pendingSIPClose = true;
    QTimer::singleShot(SoftwareInputPanelHideTimer, this,
                       SLOT(_q_inputPanelClosed()));
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
        if (mode == AnimatedTransition)
            d->rotateToAngle(angle);
        else
            d->setOrientationAngleWithoutAnimation(angle);
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

    if (event->added() && event->child()->objectName() == "debugInterface") {
        d->debugInterface = event->child();
    } else if (event->child()->objectName() == "debugInterface") {
        d->debugInterface = 0;
    }
}

#include "moc_mscenemanager.cpp"
