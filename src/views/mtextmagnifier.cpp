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

#include "mtextmagnifier.h"
#include "mdebug.h"

#include <QBitmap>
#include <QGraphicsItem>
#include <QGraphicsSceneResizeEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


MTextMagnifier::MTextMagnifier(const QGraphicsItem &sourceItem)
    : MStylableWidget(0),
      sourceItem(sourceItem)
{
    setParentItem(&overlay);
}

MTextMagnifier::~MTextMagnifier()
{
    // Before destroying parent, detach so it doesn't try to destroy us.
    setParentItem(0);
}

void MTextMagnifier::appear()
{
    // Appear in the scene of the source item.
    overlay.appear(sourceItem.scene());
    overlay.grabGesture(Qt::PanGesture);
}

void MTextMagnifier::disappear()
{
    overlay.ungrabGesture(Qt::PanGesture);
    overlay.disappear();
}

void MTextMagnifier::setMagnifiedPosition(const QPointF &sourceItemPos)
{
    setPos(sourceItem.mapToItem(parentItem(), sourceItemPos));
}

bool MTextMagnifier::isAppeared() const
{
    return overlay.isAppeared();
}

void MTextMagnifier::drawContents(QPainter *painter,
                                  const QStyleOptionGraphicsItem *option) const
{
    if (offscreenSurface.isNull()) {
        mWarning("MTextMagnifier") << "Offscreen surface not created!";
        return;
    }

    const qreal scaleFactor = 1.f + style()->magnification();

    // Source rectangle in local item coordinates.
    const QSizeF sourceSize(size() / scaleFactor);
    const QRectF sourceRect(QPointF(-sourceSize.width() / 2.f,
                                    -sourceSize.height() / 2.f),
                            sourceSize);

    // Source rectangle in source item coordinates.
    const QRectF sourceItemRect(mapRectToItem(&sourceItem, sourceRect));

    // Paint sourceItem onto offscreen surface.
    offscreenSurface->fill(Qt::transparent);
    QPainter offscreenPainter(offscreenSurface.data());

    // SmoothPixmapTransform is to draw text edit borders smoothly.
    offscreenPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Scale and then translate in scaled coordinates.
    offscreenPainter.scale(scaleFactor, scaleFactor);
    offscreenPainter.translate(-sourceItemRect.topLeft());

    QStyleOptionGraphicsItem sourceItemOption = *option;
    sourceItemOption.exposedRect = sourceItemRect;
    offscreenPainter.setCompositionMode(QPainter::CompositionMode_Source);
    const_cast<QGraphicsItem *>(&sourceItem)->paint(&offscreenPainter, &sourceItemOption);

    offscreenPainter.resetTransform();
    if (style()->magnifierMask()) {
        offscreenPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        offscreenPainter.drawPixmap(offscreenSurface->rect(), *style()->magnifierMask());
    }
    if (style()->magnifierFrame()) {
        offscreenPainter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
        offscreenPainter.drawPixmap(offscreenSurface->rect(), *style()->magnifierFrame());
    }
    offscreenPainter.end();

    // Paint the result to screen
    painter->save();

    painter->setClipRect(boundingRect(), Qt::IntersectClip);
    painter->drawPixmap(boundingRect().toRect(), *offscreenSurface);

    painter->restore();
}

void MTextMagnifier::applyStyle()
{
    if (!style()->magnifierFrame()) {
        return;
    }
    // Update bounding rectangle.
    prepareGeometryChange();
    QSize magnifierSize(style()->magnifierFrame()->size());
    if (!magnifierSize.isEmpty()) {
        resize(magnifierSize);
    }
}

void MTextMagnifier::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    // The size should always be integers already,
    // therefore safe to round.
    prepareOffscreenSurface(event->newSize().toSize());
}

QRectF MTextMagnifier::boundingRect() const
{
    // Put origo to center and apply offset from style.
    return QRectF(-rect().center() + style()->visualOffset(), size());
}

void MTextMagnifier::prepareOffscreenSurface(const QSize &size)
{
    if (size.isEmpty()
        || (offscreenSurface && offscreenSurface->size() == size)) {
        return;
    }
    offscreenSurface.reset(new QPixmap(size));
}


// Magnifier overlay widget

MagnifierOverlay::MagnifierOverlay()
{
    setFlag(QGraphicsItem::ItemHasNoContents, true);

    // Occupy whole screen to be able to catch panning gestures. The size does not really matter
    // as long as it covers whole screen in portrait and landscape.
    setManagedManually(true);
    const QSizeF visibleSceneSize = sceneManager()->visibleSceneSize(M::Landscape);
    const qreal size = qMax<qreal>(visibleSceneSize.width(), visibleSceneSize.height());
    setPreferredSize(size, size);
}

bool MagnifierOverlay::isAppeared() const
{
    return sceneWindowState() != MSceneWindow::Disappeared;
}

void MagnifierOverlay::panGestureEvent(QGestureEvent *event, QPanGesture *panGesture)
{
    // Accept gesture if magnifier is appeared. This is done to prevent panning of
    // application page but will of course prevent other uses of pan gestures as well.
    if (panGesture->state() == Qt::GestureStarted
        && isAppeared()) {
        event->accept(panGesture);
    } else {
        event->ignore(panGesture);
    }
}
