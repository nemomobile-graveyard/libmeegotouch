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
    M_VIEW(MSceneWindowModel, MSceneWindowStyle)

public:
    MStatusBarView(MStatusBar *controller);
    virtual ~MStatusBarView();

protected:
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

private:
    Q_DISABLE_COPY(MStatusBarView)

    MWidgetController *controller;
    bool updatesEnabled;

#ifdef Q_WS_X11
    void updateSharedPixmap();
#ifdef HAVE_DBUS
    bool isPixmapProviderOnline;
    QDBusServiceWatcher *dbusWatcher;
    void querySharedPixmapFromProvider();
#endif
    void setupXDamageForSharedPixmap();
    void destroyXDamageForSharedPixmap();

    QPixmap sharedPixmap;
    Qt::HANDLE pixmapDamage;

private Q_SLOTS:
    void handlePixmapDamageEvent(Qt::HANDLE &damage, short &x, short &y,
                                 unsigned short &width, unsigned short &height);

    void enablePixmapUpdates();
    void disablePixmapUpdates();
#ifdef HAVE_DBUS
    void sharedPixmapHandleReceived(QDBusPendingCallWatcher * call);
    void handlePixmapProviderOnline();
    void handlePixmapProviderOffline();
#endif

#endif

#ifdef UNIT_TEST
    friend class Ut_MStatusBarView;
#endif

};

//! \internal_end

#endif
