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

#include "mstatusbarview.h"
#include <mstatusbar.h>
#include <mapplication.h>
#include <mscenemanager.h>
#include <mdebug.h>

#include <QFile>
#include <QDataStream>
#include <QDir>

#ifdef Q_WS_X11
#include <X11/extensions/Xdamage.h>
#endif

MStatusBarView::MStatusBarView(MStatusBar *controller) :
    MSceneWindowView(controller),
    controller(controller),
    updatesEnabled(false)
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
	
    updateSharedPixmap();
#endif
}

MStatusBarView::~MStatusBarView()
{
#ifdef Q_WS_X11
    destroyXDamageForSharedPixmap();
#endif
}

void MStatusBarView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

#ifdef Q_WS_X11
    if (sharedPixmap.isNull()) {
        MStatusBarView *view = const_cast<MStatusBarView *>(this);
        view->updateSharedPixmap();
    }

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
#endif
}

#ifdef Q_WS_X11
void MStatusBarView::updateSharedPixmap()
{
    destroyXDamageForSharedPixmap();

    if (!updatesEnabled)
       return;

    Qt::HANDLE handle;
    if (fetchSharedPixmapHandle(&handle)) {
        sharedPixmap = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);
    }

    if (!sharedPixmap.isNull()) {
        setupXDamageForSharedPixmap();
    }
}

bool MStatusBarView::fetchSharedPixmapHandle(Qt::HANDLE *handle)
{
    QFile handleTempFile(QDir::temp().filePath("mstatusbar_pixmap_handle"));

    if (!handleTempFile.exists())
        return false;

    if (!handleTempFile.open(QIODevice::ReadOnly))
        return false;

    quint32 intHandle;
    QDataStream dataStream(&handleTempFile);
    dataStream >> intHandle;

    *handle = intHandle;

    return true;
}

void MStatusBarView::setupXDamageForSharedPixmap()
{
    Q_ASSERT(!sharedPixmap.isNull());

    pixmapDamage = XDamageCreate(QX11Info::display(), sharedPixmap.handle(), XDamageReportNonEmpty);
}

void MStatusBarView::destroyXDamageForSharedPixmap()
{
    if (pixmapDamage) {
        XDamageDestroy(QX11Info::display(), pixmapDamage);
        pixmapDamage = 0;
    }
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
    updateSharedPixmap();
}

void MStatusBarView::disablePixmapUpdates()
{
    updatesEnabled = false;
    destroyXDamageForSharedPixmap();
}

#endif

M_REGISTER_VIEW_NEW(MStatusBarView, MStatusBar)
