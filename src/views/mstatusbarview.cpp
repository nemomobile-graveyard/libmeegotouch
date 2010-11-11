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

#ifdef HAVE_DBUS
#include <QDBusInterface>
#include <QDBusServiceWatcher> 
#include <QDBusConnectionInterface>
#endif // HAVE_DBUS

#ifdef HAVE_XDAMAGE
#include <X11/extensions/Xdamage.h>
#endif //HAVE_XDAMAGE

#ifdef HAVE_DBUS
namespace{
    const QString PIXMAP_PROVIDER_DBUS_SERVICE = "com.meego.core.MStatusBar";
    const QString PIXMAP_PROVIDER_DBUS_PATH = "/statusbar";
    const QString PIXMAP_PROVIDER_DBUS_INTERFACE = "com.meego.core.MStatusBar";
    const QString PIXMAP_PROVIDER_DBUS_SHAREDPIXMAP_CALL = "sharedPixmapHandle";
}
#endif // HAVE_DBUS

const QString MStatusBarView::STATUS_INDICATOR_MENU_DBUS_SERVICE = "com.meego.core.MStatusIndicatorMenu";
const QString MStatusBarView::STATUS_INDICATOR_MENU_DBUS_PATH = "/statusindicatormenu";
const QString MStatusBarView::STATUS_INDICATOR_MENU_DBUS_INTERFACE = "com.meego.core.MStatusIndicatorMenu";

MStatusBarView::MStatusBarView(MStatusBar *controller) :
    MSceneWindowView(controller),
    controller(controller)
#ifdef Q_WS_X11
    , updatesEnabled(true)
    , isInSwitcher(false)
#endif
{
#ifdef Q_WS_X11
    pixmapDamage = 0;

    MApplication *mApplication = static_cast<MApplication *>(QCoreApplication::instance());
    connect(mApplication, SIGNAL(damageEvent(Qt::HANDLE&, short&, short&, unsigned short&, unsigned short&)),
            SLOT(handlePixmapDamageEvent(Qt::HANDLE&, short&, short&, unsigned short&, unsigned short&)));
    connect(controller, SIGNAL(displayExited()),
            this, SLOT(disablePixmapUpdates()));
    connect(controller, SIGNAL(displayEntered()),
            this, SLOT(enablePixmapUpdates()));


    if (controller->scene() && !controller->scene()->views().isEmpty()) {
        MWindow* win = qobject_cast<MWindow*>(controller->scene()->views().at(0));
        if (win){
            connect(win, SIGNAL(switcherEntered()), this, SLOT(handleSwitcherEntered()));
            connect(win, SIGNAL(switcherExited()), this, SLOT(handleSwitcherExited()));
        }
    }

#ifdef HAVE_DBUS
    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(PIXMAP_PROVIDER_DBUS_SERVICE))
        isPixmapProviderOnline = true;
    else
        isPixmapProviderOnline = false;

    dbusWatcher = new QDBusServiceWatcher( PIXMAP_PROVIDER_DBUS_SERVICE , QDBusConnection::sessionBus(),
                                           QDBusServiceWatcher::WatchForRegistration|QDBusServiceWatcher::WatchForUnregistration,
                                           this );

    connect(dbusWatcher, SIGNAL(serviceRegistered(QString)),
            this, SLOT(handlePixmapProviderOnline()));
    connect(dbusWatcher, SIGNAL(serviceUnregistered(QString)),
            this, SLOT(handlePixmapProviderOffline()));

    querySharedPixmapFromProvider();
#endif // HAVE_DBUS

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
#ifdef HAVE_DBUS
    if (sharedPixmap.isNull()) {
        MStatusBarView *view = const_cast<MStatusBarView *>(this);
        view->querySharedPixmapFromProvider();
    }
#endif
    if (sharedPixmap.isNull() || (controller->sceneManager() == 0))
        return;

    QRectF sourceRect;
    if (controller->sceneManager()->orientation() == M::Landscape) {
        sourceRect.setX(0);
        sourceRect.setY(0);
        sourceRect.setWidth(size().width());
        sourceRect.setHeight(size().height());
    } else {
        sourceRect.setX(0);
        sourceRect.setY(size().height());
        sourceRect.setWidth(size().width());
        sourceRect.setHeight(size().height());
    }
    
    painter->drawPixmap(QPointF(0.0, 0.0), sharedPixmap, sourceRect);
#else
    Q_UNUSED(painter);
#endif // Q_WS_X11
}

void MStatusBarView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    firstPos = event->pos();
    playHapticsFeedback();
}

void MStatusBarView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!style()->useSwipeGesture()) {
        return;
    }

    if(firstPos.y()+ style()->swipeThreshold() < event->pos().y()) {
        showStatusIndicatorMenu();
    }
}

void MStatusBarView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (style()->useSwipeGesture()) {
        return;
    }

    QPointF touch = event->scenePos();
    QRectF rect = controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    if(rect.contains(touch)) {
        showStatusIndicatorMenu();
    }
}

#ifdef Q_WS_X11
void MStatusBarView::updateSharedPixmap()
{
    destroyXDamageForSharedPixmap();
#ifdef HAVE_DBUS
    if ( (!updatesEnabled)||(!isPixmapProviderOnline)|| isInSwitcher ) {
        return;
    }
#else
    if (!updatesEnabled) {
        return;
    }
#endif // HAVE_DBUS

    if (!sharedPixmap.isNull()) {
        setupXDamageForSharedPixmap();
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
        controller->update(x, y, width, height);
    }
}

void MStatusBarView::enablePixmapUpdates()
{
    updatesEnabled = true;
#ifdef HAVE_DBUS
    querySharedPixmapFromProvider();
#endif // HAVE_DBUS
}

void MStatusBarView::disablePixmapUpdates()
{
    updatesEnabled = false;
    destroyXDamageForSharedPixmap();
}
#ifdef HAVE_DBUS
void MStatusBarView::querySharedPixmapFromProvider()
{
    if ((!updatesEnabled)||(!isPixmapProviderOnline) || isInSwitcher)
        return;
    QDBusInterface interface(PIXMAP_PROVIDER_DBUS_SERVICE, PIXMAP_PROVIDER_DBUS_PATH, PIXMAP_PROVIDER_DBUS_INTERFACE,
                             QDBusConnection::sessionBus());
    QDBusPendingCall asyncCall =  interface.asyncCall(PIXMAP_PROVIDER_DBUS_SHAREDPIXMAP_CALL);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(asyncCall, this);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(sharedPixmapHandleReceived(QDBusPendingCallWatcher*)));
}

void MStatusBarView::sharedPixmapHandleReceived(QDBusPendingCallWatcher * call)
{
    QDBusPendingReply<quint32> reply = *call;
    if (reply.isError()) {
        mWarning("MStatusBarView")<< reply.error().message();
        return;
    }
    quint32 tmp = reply;
    sharedPixmap = QPixmap::fromX11Pixmap(tmp, QPixmap::ExplicitlyShared);
    updateSharedPixmap();
    call->deleteLater();
    update();
}

void MStatusBarView::handlePixmapProviderOnline()
{
    isPixmapProviderOnline = true;
    querySharedPixmapFromProvider();
}

void MStatusBarView::handlePixmapProviderOffline()
{
    isPixmapProviderOnline = false;
    destroyXDamageForSharedPixmap();
}
#endif // HAVE_DBUS

void MStatusBarView::handleSwitcherEntered()
{
    isInSwitcher = true;
    destroyXDamageForSharedPixmap();
}

void MStatusBarView::handleSwitcherExited()
{
    isInSwitcher = false;
#ifdef HAVE_DBUS
    querySharedPixmapFromProvider();
#endif // HAVE_DBUS
}
#endif // Q_WS_X11

void MStatusBarView::showStatusIndicatorMenu()
{
#ifdef HAVE_DBUS
    QDBusInterface interface(STATUS_INDICATOR_MENU_DBUS_SERVICE, STATUS_INDICATOR_MENU_DBUS_PATH, STATUS_INDICATOR_MENU_DBUS_INTERFACE, QDBusConnection::sessionBus());
    interface.call(QDBus::NoBlock, "open");
#endif // HAVE_DBUS
}

void MStatusBarView::playHapticsFeedback()
{
    style()->pressFeedback().play();
}


M_REGISTER_VIEW_NEW(MStatusBarView, MStatusBar)
