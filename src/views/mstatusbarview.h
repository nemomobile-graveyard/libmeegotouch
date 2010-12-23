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

#ifndef MSTATUSBARVIEW_H
#define MSTATUSBARVIEW_H

#include <mscenewindowview.h>
#include <mscenewindowmodel.h>
#include <mstatusbarstyle.h>


class MStatusBar;
class QPixmap;
#ifdef HAVE_DBUS
class QDBusServiceWatcher;
class QDBusPendingCallWatcher;
#endif

//! \internal

class MStatusBarView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MStatusBarStyle)

public:
    MStatusBarView(MStatusBar *controller);
    virtual ~MStatusBarView();

protected:
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Q_DISABLE_COPY(MStatusBarView)

    MWidgetController *controller;
    /*!
     * Displays the status indicator menu.
     */
    void showStatusIndicatorMenu();

    // Perform the haptic feedback
    void playHapticsFeedback();

    //! The name of the status indicator menu D-Bus service
    static const QString STATUS_INDICATOR_MENU_DBUS_SERVICE;

    //! The name of the status indicator menu D-Bus path
    static const QString STATUS_INDICATOR_MENU_DBUS_PATH;

    //! The name of the status indicator menu D-Bus interface
    static const char *  STATUS_INDICATOR_MENU_DBUS_INTERFACE;

    //! position of mouse button press(firstPos) and position of last point of mouse move(lastPos)
    QPointF firstPos, lastPos;

#ifdef Q_WS_X11
    bool updatesEnabled;
    bool isInSwitcher;
    void updateSharedPixmap();
#ifdef HAVE_DBUS
    bool isPixmapProviderOnline;
    QDBusServiceWatcher *dbusWatcher;
    void querySharedPixmapFromProvider();
#endif // HAVE_DBUS
    void setupXDamageForSharedPixmap();
    void destroyXDamageForSharedPixmap();

    QPixmap sharedPixmap;
    Qt::HANDLE pixmapDamage;

private Q_SLOTS:
    void handlePixmapDamageEvent(Qt::HANDLE &damage, short &x, short &y,
                                 unsigned short &width, unsigned short &height);

    void enablePixmapUpdates();
    void disablePixmapUpdates();
    void handleSwitcherEntered();
    void handleSwitcherExited();
#ifdef HAVE_DBUS
    void sharedPixmapHandleReceived(QDBusPendingCallWatcher * call);
    void handlePixmapProviderOnline();
    void handlePixmapProviderOffline();
#endif // HAVE_DBUS

#endif // Q_WS_X11

#ifdef UNIT_TEST
    friend class Ut_MStatusBarView;
#endif // UNIT_TEST

};

//! \internal_end

#endif
