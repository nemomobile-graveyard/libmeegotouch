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

#include <QGraphicsItem>
#include <QGraphicsSceneResizeEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>

MTextMagnifier::MTextMagnifier(const MWidget &sourceWidget,
                               const QSizeF &keepVisibleSize)
    : MStylableWidget(0),
      sourceWidget(sourceWidget),
      keepVisibleSize(keepVisibleSize),
      overlay(sourceWidget.sceneManager())
{
    overlay.hide();
    sourceWidget.scene()->addItem(&overlay);
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
    overlay.show();
    overlay.grabGesture(Qt::PanGesture);
}

void MTextMagnifier::disappear()
{
    overlay.ungrabGesture(Qt::PanGesture);
    overlay.hide();
}

void MTextMagnifier::setMagnifiedPosition(const QPointF &sourceWidgetPos)
{
    // Map position to overlay's coordinate system.
    setPos(sourceWidget.mapToItem(parentItem(), sourceWidgetPos));

    // Position has changed, we might have to move magnifier offset
    // to stay inside screen area.
    updateMagnifierOffset();
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
    const QSizeF sourceSize(frameSize() / scaleFactor);
    const QRectF sourceRect(QPointF(-sourceSize.width() / 2.0f,
                                    -sourceSize.height() / 2.0f),
                            sourceSize);

    // Source rectangle in source item coordinates.
    const QRectF sourceWidgetRect(mapRectToItem(&sourceWidget, sourceRect));

    // Paint sourceWidget onto offscreen surface.
    offscreenSurface->fill(Qt::transparent);
    QPainter offscreenPainter(offscreenSurface.data());

    // SmoothPixmapTransform is to draw text edit borders smoothly.
    offscreenPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Scale and then translate in scaled coordinates.
    offscreenPainter.scale(scaleFactor, scaleFactor);
    offscreenPainter.translate(-sourceWidgetRect.topLeft());

    QStyleOptionGraphicsItem sourceWidgetOption = *option;
    sourceWidgetOption.exposedRect = sourceWidgetRect;
    offscreenPainter.setCompositionMode(QPainter::CompositionMode_Source);
    const_cast<MWidget *>(&sourceWidget)->paint(&offscreenPainter, &sourceWidgetOption);

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
    const QRectF frameRect(QPointF(), frameSize());
    return QRectF(-frameRect.center()
                  + offsetFromCenter, size());
}

QSizeF MTextMagnifier::frameSize() const
{
    // Assume that magnifier frame is always square. The picture
    // includes shadow and is therefore a bit larger in height.
    return QSizeF(size().width(),
                  size().width());
}

void MTextMagnifier::updateMagnifierOffset()
{
    // This method recalculates offsetFromCenter variable which is used
    // by boundingRect().
    prepareGeometryChange();

    const qreal scaleFactor = 1.0f + style()->magnification();
    const QSizeF magnifiedKeepVisibleSize(qMin<qreal>(keepVisibleSize.width() * scaleFactor,
                                                      frameSize().width()),
                                          qMin<qreal>(keepVisibleSize.height() * scaleFactor,
                                                      frameSize().height()));
    const QSizeF margins = (frameSize() - magnifiedKeepVisibleSize) / 2.0f;
    const QRectF constraint(overlay.rect().adjusted(-margins.width(), -margins.height(),
                                                    margins.width(), margins.height()));

    // Apply the constraint to bounding rect.
    // Note: following bounding rectangles are handled in parent coordinates.
    offsetFromCenter = style()->visualOffset();
    QRectF br(pos() + boundingRect().topLeft(), boundingRect().size());
    const QPointF newBrPos(qBound<qreal>(constraint.left(),
                                         br.left(),
                                         constraint.right() - br.width()),
                           qBound<qreal>(constraint.top(),
                                         br.top(),
                                         constraint.bottom() - br.height()));

    // Offset by the amount that bounding rect was moved by applying constraint.
    offsetFromCenter += newBrPos - br.topLeft();
}

void MTextMagnifier::prepareOffscreenSurface(const QSize &size)
{
    if (size.isEmpty()
        || (offscreenSurface && offscreenSurface->size() == size)) {
        return;
    }
    offscreenSurface.reset(new QPixmap(size));
}
