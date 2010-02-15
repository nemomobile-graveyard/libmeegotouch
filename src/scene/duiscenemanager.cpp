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

#include "duiscenemanager.h"
#include "duiscenemanager_p.h"

#include <DuiDebug>
#include <QMouseEvent>
#include <QTimeLine>
#include <QList>
#include <QGraphicsWidget>
#include <QEventLoop>
#include <QInputContext>
#include <QTimer>

#include <DuiOnDisplayChangeEvent>
#include "duiscenewindow.h"
#include "duiscenewindow_p.h"
#include "duideviceprofile.h"
#include "duidialog.h"
#include "duiscene.h"
#include "duiscene_p.h"
#include "duiscenelayereffect.h"
#include "duinavigationbar.h"
#include "duidockwidget.h"
#include "duiescapebuttonpanel.h"
#include "duiescapebuttonpanelmodel.h"
#include "duiapplication.h"
#include "duiwindow.h"
#include "duiapplicationwindow.h"
#include "duiapplicationwindow_p.h"

#include "duiinputmethodstate.h"

#include "duiapplicationpage.h"
#include "duiorientationtracker.h"

#include "duibasicorientationanimation.h"
#include "duinotificationanimation.h"
#include "duiscenefadeanimation.h"
#include "duiabstractwidgetanimation.h"
#include "duipageswitchanimation.h"

const int DuiSceneManagerPrivate::KeyboardSpacing = 20;

namespace
{
    //! This is a guess for the SIP close timeout since there is no general way of getting it.
    const int SoftwareInputPanelHideTimer = 500;
}


void DuiSceneManagerPrivate::init(DuiScene *scene)
{
    Q_Q(DuiSceneManager);

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

    // set the angle to that of the topmost window, if one exists
    DuiWindow *activeWindow = DuiApplication::activeWindow();

    DuiWindow *viewingWindow = 0;
    if (scene->views().size() > 0) {
        viewingWindow = qobject_cast<DuiWindow *>(scene->views().at(0));
        Q_ASSERT(viewingWindow);
    }

    if (activeWindow && activeWindow != viewingWindow)
        angle = activeWindow->orientationAngle();
    else
        angle = DuiOrientationTracker::instance()->orientationAngle();
    newAngle = angle;

    windows = new QList<DuiSceneWindow *>();
    pageHistory = new QList<DuiSceneWindow *>();

    rootElement = new QGraphicsWidget();
    rootElement->setTransformOriginPoint(QPointF(q->visibleSceneSize().width() / 2.0, q->visibleSceneSize().height() / 2.0));
    scene->addItem(rootElement);

    //TODO: get this from theme
    orientationAnimation = new DuiBasicOrientationAnimation(q->visibleSceneSize(Dui::Landscape));
    orientationAnimation->setRootElement(rootElement);
    q->connect(orientationAnimation, SIGNAL(orientationChanged()), SLOT(_q_changeGlobalOrientation()));
    q->connect(orientationAnimation, SIGNAL(finished()), SLOT(_q_emitOrientationChangeFinished()));

    pageSwitchAnimation = new DuiPageSwitchAnimation;

    setOrientationAngleWithoutAnimation(newAngle);
}

DuiSceneManagerPrivate::~DuiSceneManagerPrivate()
{
    delete windows;
    delete pageHistory;
    delete orientationAnimation;
    delete pageSwitchAnimation;
}

int DuiSceneManagerPrivate::zForWindowType(DuiSceneWindow::WindowType windowType)
{
    int z = 0;
    switch (windowType) {
    case DuiSceneWindow::PlainSceneWindow:
        z = DuiSceneManagerPrivate::PlainSceneWindow;
        break;
    case DuiSceneWindow::ApplicationPage:
        z = DuiSceneManagerPrivate::ApplicationPage;
        break;
    case DuiSceneWindow::NavigationBar:
        z = DuiSceneManagerPrivate::NavigationBar;
        break;
    case DuiSceneWindow::DockWidget:
        z = DuiSceneManagerPrivate::DockWidget;
        break;
    case DuiSceneWindow::EscapeButtonPanel:
        z = DuiSceneManagerPrivate::EscapeButtonPanel;
        break;
    case DuiSceneWindow::Dialog:
        z = DuiSceneManagerPrivate::Dialog;
        break;
    case DuiSceneWindow::ModalSceneWindow:
        z = DuiSceneManagerPrivate::ModalSceneWindow;
        break;
    case DuiSceneWindow::MessageBox:
        z = DuiSceneManagerPrivate::MessageBox;
        break;
    case DuiSceneWindow::ApplicationMenu:
        z = DuiSceneManagerPrivate::ApplicationMenu;
        break;
    case DuiSceneWindow::PopupList:
        z = DuiSceneManagerPrivate::PopupList;
        break;
    case DuiSceneWindow::NotificationInformation:
        z = DuiSceneManagerPrivate::NotificationInformation;
        break;
    case DuiSceneWindow::NotificationEvent:
        z = DuiSceneManagerPrivate::NotificationEvent;
        break;
    case DuiSceneWindow::Overlay:
        z = DuiSceneManagerPrivate::Overlay;
        break;
    case DuiSceneWindow::ObjectMenu:
        z = DuiSceneManagerPrivate::ObjectMenu;
        break;
    case DuiSceneWindow::Completer:
        z = DuiSceneManagerPrivate::Completer;
        break;
    case DuiSceneWindow::HomeButtonPanel:
        z = DuiSceneManagerPrivate::HomeButtonPanel;
        break;
    default:
        z = 0;
        // Should not get here. Only scene layer effect does not have it's
        // case statement and z is not asked for effect layer
        break;
    }
    return z;
}

void DuiSceneManagerPrivate::_q_setSenderGeometry()
{
    Q_Q(DuiSceneManager);

    DuiSceneWindow *window = qobject_cast<DuiSceneWindow *>(q->sender());
    if (window)
        setSceneWindowGeometry(window);
}

void DuiSceneManagerPrivate::_q_changeGlobalOrientation()
{
    Q_Q(DuiSceneManager);

    angle = newAngle;

    // Send a signal that the orientation is about to change
    emit q->orientationAboutToChange(q->orientation());

    // All scene windows are resized
    setSceneWindowGeometries();

    // Send a rotation event to the scene
    emit q->orientationChanged(q->orientation());
}

void DuiSceneManagerPrivate::_q_emitOrientationChangeFinished()
{
    Q_Q(DuiSceneManager);

    emit q->orientationChangeFinished(q->orientation());
}

void DuiSceneManagerPrivate::_q_unFreezeUI()
{
    Q_Q(DuiSceneManager);

    rootElement->setEnabled(true);

    QAbstractAnimation *animation = dynamic_cast<QAbstractAnimation *>(q->sender());
    if (animation == 0)
        return;

    QObject::disconnect(animation, SIGNAL(finished()), q, SLOT(_q_unFreezeUI()));
}

void DuiSceneManagerPrivate::_q_windowShowAnimationFinished()
{
    Q_Q(DuiSceneManager);
    DuiSceneWindow *window = dynamic_cast<DuiSceneWindow *>(q->sender());
    if (window == 0)
        return;

    if (isOnDisplay()) {
        produceMustBeResolvedDisplayEvent(window);
    }

    QObject::disconnect(window, SIGNAL(windowShown()), q, SLOT(_q_windowShowAnimationFinished()));
}

void DuiSceneManagerPrivate::_q_windowHideAnimationFinished()
{
    Q_Q(DuiSceneManager);
    //TODO: There could be a mapping between windows and animations so that
    //      when a window is detached all its animations are deleted as well
    //      so this method would never got called for "invalid"/detached window.
    DuiSceneWindow *window = dynamic_cast<DuiSceneWindow *>(q->sender());
    if (window == 0)
        return;

    if (window->windowType() == DuiSceneWindow::NavigationBar && !navBarHidden)
        navBar = 0;

    if (window->windowType() == DuiSceneWindow::EscapeButtonPanel && !escapeButtonHidden)
        escapeButtonPanel = 0;

    if (isOnDisplay()) {
        produceFullyOffDisplayEvents(window);
    }

    window->hide();

    QObject::disconnect(window, SIGNAL(windowHidden()), q, SLOT(_q_windowHideAnimationFinished()));

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

        if ((window->deletionPolicy() == DuiSceneWindow::DestroyWhenDone) ||
                ((window->deletionPolicy() == DuiSceneWindow::DestroyWhenDismissed)
                 && window->d_func()->dismissed)) {
            window->deleteLater();
        }
    }
}

void DuiSceneManagerPrivate::_q_relocateWindowByInputPanel(const QRect &inputPanelRect)
{
    if (focusedInputWidget) {
        const QRect widgetRect(
            focusedInputWidget->mapRectToItem(rootElement, focusedInputWidget->boundingRect()).toRect());
        const QRect mappedInputPanelRect(rootElement->mapRectFromScene(inputPanelRect).toRect());

        if (widgetRect.intersects(mappedInputPanelRect)) {
            const QRect intersection(widgetRect.intersected(mappedInputPanelRect));
            const InputPanelPlacement panelPlacement(inputPanelPlacement(mappedInputPanelRect));
            alteredSceneWindow = parentSceneWindow(focusedInputWidget);
            if (alteredSceneWindow) {
                QPoint newTranslation;
                switch (panelPlacement) {
                case North:
                    newTranslation.ry() += mappedInputPanelRect.bottomLeft().y() -
                                           intersection.topLeft().y() + KeyboardSpacing;
                    break;
                case South:
                    newTranslation.ry() += intersection.bottomLeft().y() -
                                           mappedInputPanelRect.topLeft().y() + KeyboardSpacing;
                    newTranslation.ry() *= -1;
                    break;
                default:;
                }
                moveSceneWindow(alteredSceneWindow, newTranslation);
            }
        }
    }
}

void DuiSceneManagerPrivate::_q_restoreSceneWindow()
{
    if (alteredSceneWindow && !focusedInputWidget) {
        sceneWindowTranslation *= -1;
        alteredSceneWindow->moveBy(sceneWindowTranslation.x(), sceneWindowTranslation.y());
        sceneWindowTranslation = QPoint();
        alteredSceneWindow = 0;
    }
}

Dui::Orientation DuiSceneManagerPrivate::orientation(Dui::OrientationAngle angle) const
{
    return (angle == Dui::Angle0 || angle == Dui::Angle180) ? Dui::Landscape : Dui::Portrait;
}

void DuiSceneManagerPrivate::attachWindow(DuiSceneWindow *window)
{
    if (!windows->contains(window))
        windows->append(window);

    if (window->windowType() == DuiSceneWindow::ApplicationPage)
        pageHistory->append(window);
}

void DuiSceneManagerPrivate::detachWindow(DuiSceneWindow *window)
{
    windows->removeOne(window);

    // If there is a layer effect it is deleted as well
    if (window->d_func()->effect) {
        window->setParentItem(rootElement);
        delete window->d_func()->effect;
        window->d_func()->effect = 0;
    }

    if (window->windowType() == DuiSceneWindow::ApplicationPage) {
        pageHistory->removeOne(window);
    }
}

DuiSceneLayerEffect *DuiSceneManagerPrivate::createLayerEffectForWindow(DuiSceneWindow *window)
{
    DuiSceneLayerEffect *effect = 0;

    switch (window->windowType()) {
    case DuiSceneWindow::PopupList:
    case DuiSceneWindow::Dialog:
    case DuiSceneWindow::MessageBox:
    case DuiSceneWindow::ModalSceneWindow:
    case DuiSceneWindow::ApplicationMenu:
        effect = new DuiSceneLayerEffect("messagebox");
        break;
    default:
        return NULL;
    }

    //resize the effect layer
    setSceneWindowGeometry(effect);

    // Add effect to scene via rootElement
    effect->setParentItem(rootElement);
    effect->setZValue(zForWindowType(window->windowType()));

    // Add window as child of the effect
    window->setParentItem(effect);

    window->d_func()->effect = effect;

    return effect;
}

void DuiSceneManagerPrivate::setSceneWindowGeometries()
{
    _q_restoreSceneWindow();
    const int size = windows->size();
    for (int i = 0; i < size; ++i)
        setSceneWindowGeometry(windows->at(i));
}

void DuiSceneManagerPrivate::setSceneWindowGeometry(DuiSceneWindow *window)
{
    if (window->isManagedManually())
        return;

    QPointF p = calculateSceneWindowPosition(window);
    window->setPos(p);
}


QPointF DuiSceneManagerPrivate::calculateSceneWindowPosition(DuiSceneWindow *window)
{
    Q_Q(DuiSceneManager);

    QSizeF s = q->visibleSceneSize(orientation(angle));
    Qt::Alignment alignment = window->alignment();
    QSizeF windowSize = window->effectiveSizeHint(Qt::PreferredSize);

    qreal xpos = window->x();
    qreal ypos = window->y();
    bool valid = true;

    if (alignment.testFlag(Qt::AlignLeft))
        xpos = 0;
    else if (alignment.testFlag(Qt::AlignHCenter))
        xpos = s.width() / 2 - windowSize.width() / 2;
    else if (alignment.testFlag(Qt::AlignRight))
        xpos = s.width() - windowSize.width();

    if (alignment.testFlag(Qt::AlignTop))
        ypos = 0;
    else if (alignment.testFlag(Qt::AlignVCenter))
        ypos = s.height() / 2 - windowSize.height() / 2;
    else if (alignment.testFlag(Qt::AlignBottom))
        ypos = s.height() - windowSize.height();

    if (alignment.testFlag(Qt::AlignJustify))
        valid = false;

    if (valid)
        return QPointF(xpos, ypos) + window->offset();

    return QPointF();
}

void DuiSceneManagerPrivate::rotateToAngle(Dui::OrientationAngle newAngle)
{
    if (this->newAngle == newAngle)
        return;

    orientationAnimation->stop();
    this->newAngle = newAngle;
    orientationAnimation->setTargetRotationAngle(angle, newAngle);
    orientationAnimation->start();
    freezeUIForAnimationDuration(orientationAnimation);
}

void DuiSceneManagerPrivate::setOrientationAngleWithoutAnimation(Dui::OrientationAngle newAngle)
{
    Q_Q(DuiSceneManager);

    this->newAngle = newAngle;
    _q_changeGlobalOrientation();

    QSize landscapeScreenSize = q->visibleSceneSize(Dui::Landscape);

    rootElement->setRotation(newAngle);
    if (orientation(angle) == Dui::Landscape) {
        rootElement->setPos(0, 0);
        rootElement->setTransformOriginPoint(landscapeScreenSize.width() / 2,
                                             landscapeScreenSize.height() / 2);
    } else {
        rootElement->setPos((landscapeScreenSize.width() - landscapeScreenSize.height()) / 2,
                            (landscapeScreenSize.height() - landscapeScreenSize.width()) / 2);
        rootElement->setTransformOriginPoint(landscapeScreenSize.height() / 2,
                                             landscapeScreenSize.width() / 2);
    }
}

bool DuiSceneManagerPrivate::onApplicationPage(QGraphicsItem *item)
{
    QGraphicsItem *parent = item->parentItem();
    DuiApplicationPage *page = 0;

    if (parent) {
        do {
            page = dynamic_cast<DuiApplicationPage *>(parent);
            if (page)
                return true;
            parent = parent->parentItem();
        } while (parent != 0);
    }
    return false;
}

DuiSceneWindow *DuiSceneManagerPrivate::parentSceneWindow(QGraphicsItem *item)
{
    QGraphicsItem *parent = item->parentItem();
    DuiSceneWindow *parentSceneWindow = 0;

    if (parent) {
        do {
            parentSceneWindow = dynamic_cast<DuiSceneWindow *>(parent);
            if (parentSceneWindow)
                break;
            parent = parent->parentItem();
        } while (parent != 0);
    }

    return parentSceneWindow;
}

DuiSceneManagerPrivate::InputPanelPlacement DuiSceneManagerPrivate::inputPanelPlacement(
    const QRect &inputPanelRect)
{
    Q_Q(DuiSceneManager);

    InputPanelPlacement placement = Invalid;
    QRect sceneRect(QPoint(), q->visibleSceneSize());

    if (inputPanelRect.topLeft() == sceneRect.topLeft() &&
            inputPanelRect.topRight() == sceneRect.topRight()) {
        placement = North;
    } else if (inputPanelRect.bottomLeft() == sceneRect.bottomLeft() &&
               inputPanelRect.bottomRight() == sceneRect.bottomRight()) {
        placement = South;
    }

    return placement;
}

void DuiSceneManagerPrivate::moveSceneWindow(DuiSceneWindow *window, const QPoint &translation)
{
    if (window) {
        window->moveBy(translation.x(), translation.y());
        sceneWindowTranslation += translation;
    }
}

bool DuiSceneManagerPrivate::validateSceneWindowPreAppearanceStatus(DuiSceneWindow *sceneWindow)
{
    Q_Q(DuiSceneManager);
    bool statusOk = true;

    if (sceneWindow->d_func()->shown) {
        DuiSceneManager *otherSceneManager = sceneWindow->sceneManager();
        Q_ASSERT(otherSceneManager != 0);

        if (otherSceneManager != q) {
            // Cannot be in two scenes simultaneously.

            DuiSceneWindow::DeletionPolicy currentPolicy = sceneWindow->deletionPolicy();

            otherSceneManager->hideWindowNow(sceneWindow);

            if (currentPolicy == DuiSceneWindow::DestroyWhenDone) {
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

bool DuiSceneManagerPrivate::isOnDisplay()
{
    Q_Q(DuiSceneManager);

    QList<QGraphicsView *> viewsList = q->scene()->views();
    DuiWindow *window = 0;
    bool result = false;
    int i = 0;

    while (result == false && i < viewsList.count()) {

        window = qobject_cast<DuiWindow *>(viewsList[i]);
        if (window && window->isOnDisplay()) {
            result = true;
        }

        i++;
    }

    return result;
}

void DuiSceneManagerPrivate::produceMustBeResolvedDisplayEvent(DuiSceneWindow *sceneWindow)
{
    Q_Q(DuiSceneManager);

    QRectF viewRect(QPointF(0, 0), q->visibleSceneSize());
    DuiOnDisplayChangeEvent displayEvent(DuiOnDisplayChangeEvent::MustBeResolved, viewRect);

    q->scene()->sendEvent(sceneWindow, &displayEvent);
}

void DuiSceneManagerPrivate::produceFullyOffDisplayEvents(QGraphicsItem *item)
{
    Q_Q(DuiSceneManager);
    DuiWidget *duiWidget;

    if (item->isWidget()) {
        duiWidget = qobject_cast<DuiWidget *>(static_cast<QGraphicsWidget *>(item));
        if (duiWidget) {
            QRectF visibleSceneRect(QPoint(0, 0), q->visibleSceneSize());
            DuiOnDisplayChangeEvent event(DuiOnDisplayChangeEvent::FullyOffDisplay, visibleSceneRect);
            q->scene()->sendEvent(duiWidget, &event);
        }
    }

    QList<QGraphicsItem *> childItemsList = item->childItems();
    int childItemsCount = childItemsList.count();

    for (int i = 0; i < childItemsCount; i++) {
        produceFullyOffDisplayEvents(childItemsList.at(i));
    }
}

void DuiSceneManagerPrivate::produceSceneWindowEvent(QEvent::Type type,
        DuiSceneWindow *sceneWindow,
        bool animatedTransition)
{
    Q_Q(DuiSceneManager);
    DuiSceneWindowEvent event(type, sceneWindow, animatedTransition);
    QList<QGraphicsView *> viewsList = q->scene()->views();
    DuiApplicationWindow *window = 0;
    int numberOfViews = viewsList.count();

    // FIXME:
    // Actually sending the event would require overriding customEvent() which
    // would mess ABI compatibility. Calling the event handler directly for now

    if (type == DuiSceneWindowEvent::eventTypeAppear()) {
        for (int i = 0; i < numberOfViews; i++) {
            window = qobject_cast<DuiApplicationWindow *>(viewsList[i]);
            if (window) {
                window->d_func()->sceneWindowAppearEvent(&event);
            }
        }

    } else if (type == DuiSceneWindowEvent::eventTypeDisappear()) {
        for (int i = 0; i < numberOfViews; i++) {
            window = qobject_cast<DuiApplicationWindow *>(viewsList[i]);
            if (window) {
                window->d_func()->sceneWindowDisappearEvent(&event);
            }
        }
    } else if (type == DuiSceneWindowEvent::eventTypeDismiss()) {
        for (int i = 0; i < numberOfViews; i++) {
            window = qobject_cast<DuiApplicationWindow *>(viewsList[i]);
            if (window) {
                window->d_func()->sceneWindowDismissEvent(&event);
            }
        }
    } else {
        qFatal("Unknown event type in DuiSceneManagerPrivate::produceSceneWindowEvent");
    }
}

void DuiSceneManagerPrivate::prepareWindowShow(DuiSceneWindow *window)
{
    Q_Q(DuiSceneManager);

    attachWindow(window);

    setSceneWindowGeometry(window);
    DuiSceneLayerEffect *effect = createLayerEffectForWindow(window);
    if (effect) {
        effect->enableEffect();
    } else {
        //add window to scene if not already there
        if (scene->items().indexOf(window) == -1) {
            window->setParentItem(rootElement);
            window->setZValue(zForWindowType(window->windowType()));
        }
    }

    // Check whether we are trying to show a window while it is in the middle of
    // a hide animation. If that's the case, we stop it.
    if (window->hideAnimation() &&
            window->hideAnimation()->state() != QAbstractAnimation::Stopped) {

        window->hideAnimation()->stop();
        QObject::disconnect(window, SIGNAL(windowHidden()),
                            q, SLOT(_q_windowHideAnimationFinished()));
    }

    if (window->windowType() == DuiSceneWindow::NavigationBar)
        navBar = qobject_cast<DuiNavigationBar *>(window);

    if (window->windowType() == DuiSceneWindow::EscapeButtonPanel)
        escapeButtonPanel = window;

    window->show();
    window->d_func()->shown = true;

    window->d_func()->dismissed = false;

    orientationAnimation->addSceneWindow(window);

    q->connect(window, SIGNAL(windowShown()), SLOT(_q_windowShowAnimationFinished()));
    q->connect(window, SIGNAL(repositionNeeded()), SLOT(_q_setSenderGeometry()));
}

void DuiSceneManagerPrivate::startPageSwitchAnimation(DuiSceneWindow *newPage,
        DuiSceneWindow *oldPage,
        DuiPageSwitchAnimation::PageTransitionDirection direction)
{
    pageSwitchAnimation->setNewPage(newPage);
    pageSwitchAnimation->setOldPage(oldPage);
    pageSwitchAnimation->setPageTransitionDirection(direction);

    pageSwitchAnimation->disconnect(SIGNAL(finished()));
    QObject::connect(pageSwitchAnimation, SIGNAL(finished()), newPage, SIGNAL(windowShown()));
    QObject::connect(pageSwitchAnimation, SIGNAL(finished()), oldPage, SIGNAL(windowHidden()));

    pageSwitchAnimation->start();
    freezeUIForAnimationDuration(pageSwitchAnimation);
}

void DuiSceneManagerPrivate::appearWindow(DuiSceneWindow *window,
        DuiSceneWindow::DeletionPolicy policy,
        bool animatedTransition)
{
    if (!validateSceneWindowPreAppearanceStatus(window)) {
        return;
    }

    Q_ASSERT(window->d_func()->shown == false);

    produceSceneWindowEvent(DuiSceneWindowEvent::eventTypeAppear(), window,
                            animatedTransition);
    prepareWindowShow(window);

    window->d_func()->policy = policy;

    // If the window was hidden using DuiSceneFadeAnimation it will be
    // left with an opacity of 0.
    // This might bite us back later if someone wants a window
    // to appear straight away with, let's say, 0.5 opacity. If that happens
    // we will have to think a better way of doing this DuiSceneFadeAnimation
    // - Daniel d'Andrada
    window->setOpacity(1.0);

    if (window->windowType() == DuiSceneWindow::ApplicationPage) {

        if (currentPage && currentPage != window) {
            prepareWindowHide(currentPage);
            produceSceneWindowEvent(DuiSceneWindowEvent::eventTypeDisappear(), currentPage,
                                    animatedTransition);
        }

        if (animatedTransition) {
            startPageSwitchAnimation(window, currentPage, DuiPageSwitchAnimation::LeftToRight);
        } else {
            if (currentPage && currentPage != window) {
                emit currentPage->windowHidden();
            }
            emit window->windowShown();
        }

    } else {
        if (animatedTransition && window->showAnimation()) {
            window->showAnimation()->resetToInitialState();
            window->showAnimation()->start();
        } else {
            emit window->windowShown();
        }
    }

    if (window->windowType() == DuiSceneWindow::ApplicationPage)
        currentPage = window;
}

void DuiSceneManagerPrivate::prepareWindowHide(DuiSceneWindow *window)
{
    Q_Q(DuiSceneManager);

    QObject::disconnect(window, SIGNAL(repositionNeeded()),
                        q, SLOT(_q_setSenderGeometry()));
    orientationAnimation->removeSceneWindow(window);
    window->d_func()->shown = false;

    if (window->d_func()->effect) {
        window->d_func()->effect->disableEffect();
    }

    // Check whether we are trying to hide a window while it is in the middle of
    // a show animation. If that's the case, we stop it.
    if (window->showAnimation() &&
            window->showAnimation()->state() != QAbstractAnimation::Stopped) {

        window->showAnimation()->stop();
        QObject::disconnect(window, SIGNAL(windowShown()),
                            q, SLOT(_q_windowShowAnimationFinished()));
    }

    q->connect(window, SIGNAL(windowHidden()), SLOT(_q_windowHideAnimationFinished()));
}

void DuiSceneManagerPrivate::disappearWindow(DuiSceneWindow *window,
        bool animatedTransition)
{
    Q_Q(DuiSceneManager);

    produceSceneWindowEvent(DuiSceneWindowEvent::eventTypeDisappear(), window,
                            animatedTransition);
    prepareWindowHide(window);

    if (animatedTransition && window->hideAnimation()) {
        window->hideAnimation()->start();
    } else {
        emit window->windowHidden();
        q->scene()->update(); // is this really needed?
    }
}

void DuiSceneManagerPrivate::freezeUIForAnimationDuration(QAbstractAnimation *animation)
{
    Q_Q(DuiSceneManager);

    if (animation->state() == QAbstractAnimation::Running) {
        //rootElement->setEnabled(false);
        QObject::connect(animation, SIGNAL(finished()), q, SLOT(_q_unFreezeUI()));
    }
}

void DuiSceneManagerPrivate::dismissWindow(DuiSceneWindow *window,
        bool animatedTransition)
{
    Q_Q(DuiSceneManager);

    produceSceneWindowEvent(DuiSceneWindowEvent::eventTypeDismiss(), window,
                            animatedTransition);
    prepareWindowHide(window);

    window->d_func()->dismissed = true;

    if (window->windowType() == DuiSceneWindow::ApplicationPage) {
        pageHistory->removeLast();
        prepareWindowShow(pageHistory->last());
        produceSceneWindowEvent(DuiSceneWindowEvent::eventTypeAppear(), pageHistory->last(),
                                animatedTransition);

        if (animatedTransition) {
            startPageSwitchAnimation(pageHistory->last(), window, DuiPageSwitchAnimation::RightToLeft);
        } else {
            emit window->windowHidden();
            emit pageHistory->last()->windowShown();
        }

        currentPage = pageHistory->last();

    } else if (animatedTransition && window->hideAnimation()) { // Fallback to legacy hide anim.
        window->hideAnimation()->start();
    } else {
        emit window->windowHidden();
        q->scene()->update(); // is this really needed?
    }
}

void DuiSceneManagerPrivate::_q_inputPanelOpened()
{
    Q_Q(DuiSceneManager);

    Q_ASSERT(focusedInputWidget);
    if (!focusedInputWidget) {
        return;
    }

    const bool widgetOnPage = onApplicationPage(focusedInputWidget);
    if (navBar && widgetOnPage) {
        navBarHidden = true;
        q->hideWindow(navBar);
    }
    if (escapeButtonPanel && (widgetOnPage || (navBar && navBar->isAncestorOf(focusedInputWidget)))) {
        escapeButtonHidden = true;
        q->hideWindow(escapeButtonPanel);
    }

    DuiInputMethodState *inputMethodState = DuiInputMethodState::instance();
    _q_relocateWindowByInputPanel(inputMethodState->inputMethodArea());
    QObject::connect(inputMethodState, SIGNAL(inputMethodAreaChanged(QRect)),
                     q, SLOT(_q_relocateWindowByInputPanel(QRect)));
}

void DuiSceneManagerPrivate::_q_inputPanelClosed()
{
    Q_Q(DuiSceneManager);

    if (!pendingSIPClose) {
        return;
    }

    focusedInputWidget = 0;

    if (navBar && navBarHidden) {
        q->showWindow(navBar);
        navBarHidden = false;
    }
    if (escapeButtonPanel && escapeButtonHidden) {
        q->showWindow(escapeButtonPanel);
        escapeButtonHidden = false;
    }

    QObject::disconnect(DuiInputMethodState::instance(),
                        SIGNAL(inputMethodAreaChanged(QRect)),
                        q, SLOT(_q_relocateWindowByInputPanel(QRect)));
    _q_restoreSceneWindow();
}

DuiSceneManager::DuiSceneManager(DuiScene *scene, QObject *parent) :
    QObject(parent), d_ptr(new DuiSceneManagerPrivate)
{
    Q_D(DuiSceneManager);

    if (scene == 0)
        scene = new DuiScene(this);

    d->q_ptr = this;
    d->init(scene);
    d->scene->d_ptr->setSceneManager(this);
}

DuiSceneManager::~DuiSceneManager()
{
    Q_D(DuiSceneManager);

    delete d;
}

DuiScene *DuiSceneManager::scene()
{
    Q_D(DuiSceneManager);

    return d->scene;
}

void DuiSceneManager::showWindow(DuiSceneWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiSceneManager);

    d->appearWindow(window, policy, true);
}

void DuiSceneManager::showWindowNow(DuiSceneWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiSceneManager);
    d->appearWindow(window, policy, false);
}

int DuiSceneManager::execDialog(DuiDialog *dialog)
{
    QEventLoop eventLoop;
    QPointer<DuiDialog> dialog_ptr = dialog;
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

    showWindow(dialog);
    eventLoop.exec();

    // use QPointer in case of the dialog being deleted in the meantime
    if (dialog_ptr)
        return dialog->standardButton(dialog->clickedButton());

    return DuiDialog::Rejected;
}

void DuiSceneManager::hideWindow(DuiSceneWindow *window)
{
    Q_D(DuiSceneManager);
    d->disappearWindow(window, true);
}

void DuiSceneManager::hideWindowNow(DuiSceneWindow *window)
{
    Q_D(DuiSceneManager);
    d->disappearWindow(window, false);
}

void DuiSceneManager::closeWindow(DuiSceneWindow *window)
{
    Q_D(DuiSceneManager);
    d->dismissWindow(window, true);
}

void DuiSceneManager::closeWindowNow(DuiSceneWindow *window)
{
    Q_D(DuiSceneManager);
    d->dismissWindow(window, false);
}

bool DuiSceneManager::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);

    event->ignore();
    return false;
}

void DuiSceneManager::requestSoftwareInputPanel(QGraphicsWidget *inputWidget)
{
    Q_D(DuiSceneManager);

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
        // Also, it is just a mere assumption that SIP will be opened in the first place.
        // Connecting a signal from DuiInputContext to DuiSceneManager is intentionally left out.
        d->pendingSIPClose = false;
        QTimer::singleShot(0, this, SLOT(_q_inputPanelOpened()));

        QWidget *focusWidget = QApplication::focusWidget();

        if (focusWidget) {
            // FIXME: this is a temporary workaround because of the
            // QGraphicsView unable to correctly update the attribute.
            // We're waiting for fixing this on Qt side.
            focusWidget->setAttribute(Qt::WA_InputMethodEnabled, true);
            //enforce update if focus is moved from one DuiTextEdit to other
            //if attribute WA_InputMethodEnabled is not set then Qt will call
            //setFocusWidget automatically
            inputContext->setFocusWidget(focusWidget);
        }

        //FIXME: verify if application style allows SIP usage
        QEvent request(QEvent::RequestSoftwareInputPanel);
        inputContext->filterEvent(&request);
    }
}

void DuiSceneManager::closeSoftwareInputPanel()
{
    Q_D(DuiSceneManager);

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
    // that both wants a SIP. DuiInputContext has its own logic for this because it cannot
    // rely that it's running on a DuiApplication.
    // Our guess now is that SIP will be closed after some delay.
    d->pendingSIPClose = true;
    QTimer::singleShot(SoftwareInputPanelHideTimer, this,
                       SLOT(_q_inputPanelClosed()));
}

void DuiSceneManager::setOrientationAngle(Dui::OrientationAngle angle,
        Dui::OrientationChangeMode mode)
{
    Q_D(DuiSceneManager);

    if (mode == Dui::AnimatedOrientationChange)
        d->rotateToAngle(angle);
    else
        d->setOrientationAngleWithoutAnimation(angle);
}

Dui::Orientation DuiSceneManager::orientation() const
{
    Q_D(const DuiSceneManager);

    return d->orientation(d->newAngle);
}

Dui::OrientationAngle DuiSceneManager::orientationAngle() const
{
    Q_D(const DuiSceneManager);

    return d->newAngle;
}

QSize DuiSceneManager::visibleSceneSize(Dui::Orientation orientation) const
{
    QSize s;

    if (orientation == Dui::Landscape) {
        s = DuiDeviceProfile::instance()->resolution();
    } else {
        s = QSize(DuiDeviceProfile::instance()->resolution().height(),
                  DuiDeviceProfile::instance()->resolution().width());
    }

    return s;
}

QSize DuiSceneManager::visibleSceneSize() const
{
    return visibleSceneSize(orientation());
}

#include "moc_duiscenemanager.cpp"
