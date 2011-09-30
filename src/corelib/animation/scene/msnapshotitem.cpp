/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "msnapshotitem.h"

#include <mgraphicssystemhelper.h>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QGraphicsView>
#include <QGLFramebufferObject>

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
#include <QtMeeGoGraphicsSystemHelper>
#endif

MSnapshotItem::MSnapshotItem(const QRectF &sceneTargetRect, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_boundingRect(sceneTargetRect), pixmap(), framebufferObject(0)
{
}

MSnapshotItem::~MSnapshotItem()
{
    delete framebufferObject;
    framebufferObject = 0;
}

QRectF MSnapshotItem::boundingRect() const
{
    return m_boundingRect;
}

void MSnapshotItem::updateSnapshot()
{
    pixmap = QPixmap();
    delete framebufferObject;
    framebufferObject = 0;

    if (scene() && scene()->views().count() == 0)
        return;

    QGraphicsView *graphicsView = scene()->views().at(0);
    Q_ASSERT(graphicsView);

    bool grabContent = true;
    if (MGraphicsSystemHelper::isRunningMeeGoGraphicsSystem() &&
        QGLContext::currentContext())
    {
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
        graphicsView->installEventFilter(this);
#endif
        const QRect rect = m_boundingRect.toRect();
        framebufferObject = new QGLFramebufferObject(rect.size(), QGLFramebufferObject::CombinedDepthStencil);
        if (framebufferObject->isValid()) {
            QPainter painter(framebufferObject);
            graphicsView->render(&painter, QRectF(), rect);
            grabContent = false;
        }
    }

    if (grabContent) {
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
        pixmap = QPixmap::grabWidget(graphicsView);
#else
        pixmap = QPixmap::grabWindow(graphicsView->effectiveWinId());
#endif
    }
}

void MSnapshotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (framebufferObject && framebufferObject->isValid()) {
        framebufferObject->drawTexture(m_boundingRect, framebufferObject->texture());
    } else if (!pixmap.isNull()) {
        painter->drawPixmap(0,0, pixmap);
    }
}

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
bool MSnapshotItem::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QMeeGoSwitchEvent::eventNumber()) {
        QMeeGoSwitchEvent* switchEvent = static_cast<QMeeGoSwitchEvent*>(event);
        if (switchEvent->state() == QMeeGoSwitchEvent::WillSwitch) {
            delete framebufferObject;
            framebufferObject = 0;
        }
    }

    return QGraphicsObject::eventFilter(obj, event);
}
#endif
