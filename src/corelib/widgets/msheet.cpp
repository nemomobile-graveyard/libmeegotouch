/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "msheet.h"
#include "msheetmodel.h"
#include "msheet_p.h"
#include "msheetstandalonewindow.h"
#include <mscene.h>
#include <mscenemanager.h>
#include <mscenemanager_p.h>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MSheet)

//////////////////
// MSheetPrivate

MSheetPrivate::MSheetPrivate()
    : standAloneWindow(0),
    appearSystemwideDeletionPolicy(0),
    statusBarVisibleInSystemwide(false),
    systemwideModeOrientation(MSheet::FollowsDeviceOrientation)
{
}

MSheetPrivate::~MSheetPrivate()
{
    delete standAloneWindow;
    standAloneWindow = 0;

    delete appearSystemwideDeletionPolicy;
    appearSystemwideDeletionPolicy = 0;
}

bool MSheetPrivate::canDisappear()
{
    Q_Q(MSheet);

    if (standAloneWindow && standAloneWindow->scene() == q->scene()
            && standAloneWindow->isVisible()) {

        // we are being shown systemwide and disappearance is being triggered
        // from application code (e.g. from the click of a "Cancel" button in
        // the header) instead of due to the stand alone window being hidden or
        // closed.
        //
        // For system-wide sheets we let the window manager handle our disappearance
        // animation (i.e. it will animate the stand alone window).
        // The scene window animation is not used at all and is therefore
        // denied here.

        standAloneWindow->close();

        return false;
    } else {
        return true;
    }
}

bool MSheetPrivate::canDismiss()
{
    return canDisappear();
}

void MSheetPrivate::_q_makeSystemSheetDisappearImmediately()
{
    Q_Q(MSheet);

    Q_ASSERT(standAloneWindow->scene() == q->scene());
    q->sceneManager()->disappearSceneWindowNow(q);
}

void MSheetPrivate::_q_dismissSystemSheetImmediately()
{
    Q_Q(MSheet);

    Q_ASSERT(standAloneWindow->scene() == q->scene());
    q->sceneManager()->dismissSceneWindowNow(q);
}

void MSheetPrivate::_q_onCentralWidgetDestroyed()
{
    Q_Q(MSheet);
    q->model()->setCentralWidget(0);
}

void MSheetPrivate::_q_onHeaderWidgetDestroyed()
{
    Q_Q(MSheet);
    q->model()->setHeaderWidget(0);
}

void MSheetPrivate::_q_onStandAloneSheetDisappeared()
{
    Q_Q(MSheet);
    Q_ASSERT(standAloneWindow != 0);
    Q_ASSERT(!standAloneWindow->isVisible());

    standAloneWindow->setSheet(0);

    q->disconnect(SIGNAL(disappeared()), q, SLOT(_q_onStandAloneSheetDisappeared()));

    // Remove sheet from scene otherwise scene will delete sheet
    // on scene's destructor

    standAloneWindow->sceneManager()->d_func()->removeSceneWindow(q);

    if (q->scene()) {
        q->scene()->removeItem(q);
    }

    if (*appearSystemwideDeletionPolicy == MSceneWindow::DestroyWhenDone
        || (*appearSystemwideDeletionPolicy == MSceneWindow::DestroyWhenDismissed && dismissed)) {

        q->deleteLater();
    }

    delete appearSystemwideDeletionPolicy;
    appearSystemwideDeletionPolicy = 0;
}

void MSheetPrivate::appearSystemwide(MSceneWindow::DeletionPolicy policy)
{
    Q_Q(MSheet);

    if (standAloneWindow == 0) {
        standAloneWindow = new MSheetStandAloneWindow;
    }

    applySystemwideModeOrientation();

    standAloneWindow->setSheet(q);
    standAloneWindow->setStatusBarVisible(statusBarVisibleInSystemwide);

    q->connect(q, SIGNAL(disappeared()), SLOT(_q_onStandAloneSheetDisappeared()));

    if (!appearSystemwideDeletionPolicy) {
        appearSystemwideDeletionPolicy = new MSceneWindow::DeletionPolicy;
    }
    *appearSystemwideDeletionPolicy = policy;

    standAloneWindow->sceneManager()->appearSceneWindowNow(q);
    standAloneWindow->updateChainTaskData();
    standAloneWindow->show();
}

void MSheetPrivate::applySystemwideModeOrientation()
{
    Q_ASSERT(standAloneWindow);

    switch (systemwideModeOrientation) {
        case MSheet::FollowsDeviceOrientation:
            standAloneWindow->unlockOrientation();
            standAloneWindow->setProperty(
                        "followsCurrentApplicationWindowOrientation", QVariant());
            break;
        case MSheet::FollowsCurrentAppWindowOrientation:
            standAloneWindow->unlockOrientation();
            standAloneWindow->setProperty(
                        "followsCurrentApplicationWindowOrientation", 1);
            break;
        case MSheet::LockedToPortraitOrientation:
            standAloneWindow->unlockOrientation();
            standAloneWindow->setProperty(
                        "followsCurrentApplicationWindowOrientation", QVariant());
            standAloneWindow->setPortraitOrientation();
            standAloneWindow->lockOrientation();
            break;
        case MSheet::LockedToLandscapeOrientation:
            standAloneWindow->unlockOrientation();
            standAloneWindow->setProperty(
                        "followsCurrentApplicationWindowOrientation", QVariant());
            standAloneWindow->setLandscapeOrientation();
            standAloneWindow->lockOrientation();
            break;
        default:
            qFatal("invalid enum value");
    };
}

//////////////////
// MSheet

MSheet::MSheet() :
    MSceneWindow(new MSheetPrivate,
                 new MSheetModel,
                 MSceneWindow::Sheet, QString(""), 0)
{
}

MSheet::~MSheet()
{
    Q_D(MSheet);

    setHeaderWidget(0);
    setCentralWidget(0);

    if (d->standAloneWindow) {
        if (d->standAloneWindow->isVisible()) {
            d->standAloneWindow->setSheet(0);
            disconnect(SIGNAL(disappeared()), this, SLOT(_q_onStandAloneSheetDisappeared()));
            d->standAloneWindow->scene()->removeItem(this);
            d->standAloneWindow->close();
        }
    }
}

QGraphicsWidget *MSheet::centralWidget()
{
    return model()->centralWidget();
}

void MSheet::setCentralWidget(QGraphicsWidget *newWidget)
{
    QGraphicsWidget *oldWidget  = model()->centralWidget();

    if (newWidget == oldWidget)
        return;

    if (oldWidget)
        disconnect(oldWidget, SIGNAL(destroyed()), this, SLOT(_q_onCentralWidgetDestroyed()));

    model()->setCentralWidget(newWidget);

    delete oldWidget;

    if (newWidget)
        connect(newWidget, SIGNAL(destroyed()), SLOT(_q_onCentralWidgetDestroyed()));
}

QGraphicsWidget *MSheet::headerWidget()
{
    return model()->headerWidget();
}

void MSheet::setHeaderWidget(QGraphicsWidget *newWidget)
{
    QGraphicsWidget *oldWidget  = model()->headerWidget();

    if (newWidget == oldWidget)
        return;

    if (oldWidget)
        disconnect(oldWidget, SIGNAL(destroyed()), this, SLOT(_q_onHeaderWidgetDestroyed()));

    model()->setHeaderWidget(newWidget);

    delete oldWidget;

    if (newWidget)
        connect(newWidget, SIGNAL(destroyed()), SLOT(_q_onHeaderWidgetDestroyed()));
}

void MSheet::appearSystemwide(MSceneWindow::DeletionPolicy policy)
{
    Q_D(MSheet);
    d->appearSystemwide(policy);
}

void MSheet::setHeaderVisible(bool visible)
{
    model()->setHeaderVisible(visible);
}

bool MSheet::isHeaderVisible() const
{
    return model()->headerVisible();
}

void MSheet::setHeaderFloating(bool floating)
{
    model()->setHeaderFloating(floating);
}

bool MSheet::isHeaderFloating() const
{
    return model()->headerFloating();
}

void MSheet::setStatusBarVisibleInSystemwide(bool visible)
{
    Q_D(MSheet);
    d->statusBarVisibleInSystemwide = visible;
}

bool MSheet::isStatusBarVisibleInSystemwide() const
{
    Q_D(const MSheet);
    return d->statusBarVisibleInSystemwide;
}

MSheet::SystemwideModeOrientation MSheet::systemwideModeOrientation() const
{
    Q_D(const MSheet);
    return d->systemwideModeOrientation;
}

void MSheet::setSystemwideModeOrientation(MSheet::SystemwideModeOrientation orientation)
{
    Q_D(MSheet);

    if (orientation == d->systemwideModeOrientation)
        return;

    d->systemwideModeOrientation = orientation;

    if (d->standAloneWindow) {
        d->applySystemwideModeOrientation();
    }
}

bool MSheet::isCentralWidgetSizePolicyRespected() const
{
    return model()->centralWidgetSizePolicyRespected();
}

void MSheet::setCentralWidgetSizePolicyRespected(bool enabled)
{
    model()->setCentralWidgetSizePolicyRespected(enabled);
}

#include "moc_msheet.cpp"
