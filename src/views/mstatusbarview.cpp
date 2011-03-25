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

#include "mviewconstants.h"
#include "mstatusbarview.h"
#include <mstatusbar.h>
#include <mapplication.h>
#include <mscenemanager.h>
#include <mscene.h>
#include <mdebug.h>

#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QGraphicsSceneMouseEvent>

#include <mstatusbarviewdbuswrapper.h>

#ifdef HAVE_XDAMAGE
#include <X11/extensions/Xdamage.h>
#endif //HAVE_XDAMAGE

namespace{
    const qreal SharedPixmapHeight = 30;
}

MStatusBarView::MStatusBarView(MStatusBar *controller) :
    MSceneWindowView(controller),
    controller(controller),
    pressDown(false)
#ifdef Q_WS_X11
    , isOnDisplay(false)
    , isInSwitcher(false)
#endif
{
#ifdef Q_WS_X11
    pixmapDamage = 0;

    MApplication *mApplication = static_cast<MApplication *>(QCoreApplication::instance());
    connect(mApplication, SIGNAL(damageEvent(Qt::HANDLE&, short&, short&, unsigned short&, unsigned short&)),
            SLOT(handlePixmapDamageEvent(Qt::HANDLE&, short&, short&, unsigned short&, unsigned short&)));
    connect(controller, SIGNAL(displayExited()),
            this, SLOT(handleDisplayExited()));
    connect(controller, SIGNAL(displayEntered()),
            this, SLOT(handleDisplayEntered()));
    controller->setProperty("sharedPixmapHeight", SharedPixmapHeight);


    if (controller->scene() && !controller->scene()->views().isEmpty()) {
        MWindow* win = qobject_cast<MWindow*>(controller->scene()->views().at(0));
        if (win) {
            isInSwitcher = win->isInSwitcher();
            connect(win, SIGNAL(switcherEntered()), this, SLOT(handleSwitcherEntered()));
            connect(win, SIGNAL(switcherExited()), this, SLOT(handleSwitcherExited()));
        }
    }

    dbusWrapper = new MStatusBarViewDBusWrapper(this);

    connect(dbusWrapper,
            SIGNAL(sharedPixmapHandleFromProviderReceived(quint32, bool)),
            SLOT(handleSharedPixmapHandleReceived(quint32, bool)));

    if (dbusWrapper->isPixmapProviderServiceRegistered())
        isPixmapProviderOnline = true;
    else
        isPixmapProviderOnline = false;


    connect(dbusWrapper, SIGNAL(pixmapProviderServiceRegistered()),
            this, SLOT(handlePixmapProviderOnline()));
    connect(dbusWrapper, SIGNAL(pixmapProviderServiceUnregistered()),
            this, SLOT(handlePixmapProviderOffline()));

    if (isPixmapProviderOnline)
        dbusWrapper->querySharedPixmapHandleFromProvider();

#endif // Q_WS_X11
}

MStatusBarView::~MStatusBarView()
{
#ifdef Q_WS_X11
    destroyXDamageForSharedPixmap();
#endif // Q_WS_X11
}

void MStatusBarView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

#ifdef Q_WS_X11
    if (sharedPixmap.isNull() && !dbusWrapper->havePendingSharedPixmapHandleReply() && isPixmapProviderOnline) {
        dbusWrapper->querySharedPixmapHandleFromProvider();
    }

    if (sharedPixmap.isNull() || (controller->sceneManager() == 0))
        return;

    QRectF sourceRect;

    sourceRect.setX(0);
    sourceRect.setY(controller->sceneManager()->orientation() == M::Landscape ? 0 : SharedPixmapHeight);
    sourceRect.setWidth(size().width());
    sourceRect.setHeight(SharedPixmapHeight);

    // provider can die under mysterious circumstances so sharedPixmap can be invalid
    try {
        painter->drawPixmap(QPointF(0.0, 0.0), sharedPixmap, sourceRect);
    } catch(...) {
        qWarning() << "MStatusBarView::drawContents: Cannot draw sharedPixmap.";
        const_cast<MStatusBarView*>(this)->sharedPixmap = QPixmap();
    }

    if (pressDown) {
        painter->save();
        painter->setOpacity(style()->pressDimFactor());
        painter->fillRect(QRectF(QPointF(0.0, 0.0), sourceRect.size()), Qt::black);
        painter->restore();
    }

#else
    Q_UNUSED(painter);
#endif // Q_WS_X11
}

void MStatusBarView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    firstPos = event->pos();
    playHapticsFeedback();

    if (pressDown)
        return;

    pressDown = true;
    update();
}

void MStatusBarView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (style()->useSwipeGesture()) {
        if(firstPos.y()+ style()->swipeThreshold() < event->pos().y()) {
            showStatusIndicatorMenu();
        }
    } else {
        QPointF touch = event->scenePos();
        // shape includes the reactive margins
        QRectF rect = controller->mapToScene(controller->shape()).boundingRect();
        rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                    M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
        if (rect.contains(touch)) {
            if (!pressDown) {
                pressDown = true;
                update();
            }
        } else {
            if (pressDown) {
                pressDown = false;
                update();
            }
        }
    }
}

void MStatusBarView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!pressDown)
        return;

    pressDown = false;
    update();

    if (style()->useSwipeGesture()) {
        return;
    }

    QPointF touch = event->scenePos();
    // shape includes the reactive margins
    QRectF rect = controller->mapToScene(controller->shape()).boundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    if(rect.contains(touch)) {
        showStatusIndicatorMenu();
    }
}

#ifdef Q_WS_X11
bool MStatusBarView::shouldStayUpToDate()
{
    return isOnDisplay && isPixmapProviderOnline && !isInSwitcher;
}

void MStatusBarView::updateXDamageForSharedPixmap()
{
    if (shouldStayUpToDate()) {
        if (!pixmapDamage) {
            if (!sharedPixmap.isNull()) {
                setupXDamageForSharedPixmap();
            } else {
                mWarning("MStatusBarView")
                    << "Couldn't track XDamage events since there's no shared pixmap.";
            }
        }
    } else {
        destroyXDamageForSharedPixmap();
    }
}

void MStatusBarView::setupXDamageForSharedPixmap()
{
    Q_ASSERT(!sharedPixmap.isNull());
#ifdef HAVE_XDAMAGE
    pixmapDamage = XDamageCreate(QX11Info::display(), sharedPixmap.handle(), XDamageReportNonEmpty);
#endif //HAVE_XDAMAGE
}

void MStatusBarView::destroyXDamageForSharedPixmap()
{
#ifdef HAVE_XDAMAGE
    if (pixmapDamage) {
        XDamageDestroy(QX11Info::display(), pixmapDamage);
        pixmapDamage = 0;
    }
#endif //HAVE_XDAMAGE
}

void MStatusBarView::handlePixmapDamageEvent(Qt::HANDLE &damage, short &x, short &y,
                                             unsigned short &width, unsigned short &height)
{
    if (damage == pixmapDamage) {
        Q_ASSERT(shouldStayUpToDate());
        controller->update(x, y, width, height);
    }
}

void MStatusBarView::handleDisplayEntered()
{
    isOnDisplay = true;
    updateXDamageForSharedPixmap();
}

void MStatusBarView::handleDisplayExited()
{
    isOnDisplay = false;
    updateXDamageForSharedPixmap();
}

void MStatusBarView::handleSharedPixmapHandleReceived(quint32 handle, bool ok)
{
    Q_ASSERT(isPixmapProviderOnline);

    if (!ok) {
        return;
    }

    sharedPixmap = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);

    destroyXDamageForSharedPixmap();
    updateXDamageForSharedPixmap();

    if (shouldStayUpToDate()) {
        update();
    }
}

void MStatusBarView::handlePixmapProviderOnline()
{
    isPixmapProviderOnline = true;

    Q_ASSERT(sharedPixmap.isNull());
    Q_ASSERT(!pixmapDamage);

    dbusWrapper->querySharedPixmapHandleFromProvider();
}

void MStatusBarView::handlePixmapProviderOffline()
{
    isPixmapProviderOnline = false;

    destroyXDamageForSharedPixmap();
    sharedPixmap = QPixmap();
}

void MStatusBarView::handleSwitcherEntered()
{
    isInSwitcher = true;
    updateXDamageForSharedPixmap();
}

void MStatusBarView::handleSwitcherExited()
{
    isInSwitcher = false;
    updateXDamageForSharedPixmap();
}
#endif // Q_WS_X11

void MStatusBarView::showStatusIndicatorMenu()
{
    if (style()->enableStatusIndicatorMenu()) {
        dbusWrapper->openStatusIndicatorMenu();
    }
}

void MStatusBarView::playHapticsFeedback()
{
    if (style()->enableStatusIndicatorMenu()) {
        style()->pressFeedback().play();
    }
}

M_REGISTER_VIEW_NEW(MStatusBarView, MStatusBar)
