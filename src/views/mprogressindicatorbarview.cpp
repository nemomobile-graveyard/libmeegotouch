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

#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsSceneResizeEvent>
#include <QApplication>
#include <QTimer>
#include <QDebug>

#include "mprogressindicatorbarview.h"
#include "mprogressindicatorbarview_p.h"
#include "mprogressindicator.h"
#include "mviewcreator.h"
#include "mprogressindicator_p.h" // For the member indexes of the model
#include "mscalableimage.h"
#include "mtheme.h"
#include "mdebug.h"

MProgressIndicatorBarViewPrivate::MProgressIndicatorBarViewPrivate()
    :  leftWidth(0),
       rightWidth(0),
       top(0),
       bottom(0),
       q_ptr(0),
       controller(0),
       elementSize(0),
       activeElementCount(0),
       position(0),
       width(0),
       paused(true),
       timer(0),
       previousValue(0),
       animationCacheSize(0)
{
    backgroundPainter = new QPainter;
    scalableBarImage = new MScalableImage;
    timer = new QTimer;
}


MProgressIndicatorBarViewPrivate::~MProgressIndicatorBarViewPrivate()
{
    delete backgroundPainter;
    delete scalableBarImage;
}

bool MProgressIndicatorBarViewPrivate::textureTiled() const
{
    Q_Q(const MProgressIndicatorBarView);
    if( q->model()->unknownDuration() )
        return q->style()->unknownBarTextureTiled();
    else
        return q->style()->knownBarTextureTiled();
}

bool MProgressIndicatorBarViewPrivate::fullWidth() const
{
    Q_Q(const MProgressIndicatorBarView);
    return (q->style()->unknownBarSize().width() >= q->rect().width());
}

void MProgressIndicatorBarViewPrivate::setPosition()
{
    Q_Q(MProgressIndicatorBarView);

    if(paused)
        return;

    position++;
    if(fullWidth() && position>=animationCacheSize)
        position=0;
    else if (!fullWidth() && position>=q->rect().width())
        position=0;

    q->update(q->rect());
}

qreal MProgressIndicatorBarViewPrivate::getPosition()
{
    return position;
}


void MProgressIndicatorBarViewPrivate::animate(bool animate)
{
    Q_Q(MProgressIndicatorBarView);

    animate = (animate && q->model()->unknownDuration());
    if (animate) {
        paused = false;
        timer->start();
    } else {
        paused = true;
        timer->stop();
    }
}

void MProgressIndicatorBarViewPrivate::createMaskOnGeometry()
{
    Q_Q(MProgressIndicatorBarView);

    const int barHeight = q->rect().height();
    const QPoint topLeft(0,0);
    QPainter p;

    /*
       leftEnd      barMask      rightEnd
       +-----+-------+---+--------+-----+
       |     |       |///|        |     |
       |     |       |///|        |     |
       |     |       |///|        |     |
       +-----+-------+---+--------+-----+
     */

    QImage canvas(q->rect().width(),barHeight, QImage::Format_ARGB32);
    canvas.fill(Qt::transparent);

    leftEndMask = QImage(leftWidth,barHeight,QImage::Format_ARGB32);
    leftEndMask.fill(Qt::transparent);

    rightEndMask = QImage(rightWidth,barHeight,QImage::Format_ARGB32);
    rightEndMask.fill(Qt::transparent);

    // first scale up and draw the entire mask image
    p.begin( &canvas );
    q->style()->progressBarMask()->draw(q->rect().toRect(), &p);
    p.end();

    // then cut out left end
    p.begin(&leftEndMask);
    p.drawImage( QRect(topLeft,leftEndMask.size()), canvas, QRect(topLeft,QSize(leftWidth,barHeight)) );
    p.end();

    // and cut out right end
    p.begin(&rightEndMask);
    p.drawImage( QRect(topLeft,rightEndMask.size()), canvas, QRect(QPoint(q->rect().toRect().width()-rightWidth,0),QSize(rightWidth,barHeight)) );
    p.end();

    barMask = QImage( QSize(q->style()->knownBarTexture()->pixmap()->width(),barHeight),QImage::Format_ARGB32);
    barMask.fill(Qt::transparent);

    // alpha blending is more correct in the middle
    const QPoint centerTop( q->rect().center().x() - barMask.size().width()/2, q->rect().y() );
    p.begin(&barMask);
    p.drawImage( QRect(QPoint(0,0),barMask.size()), canvas, QRect(centerTop, barMask.size()) );
    p.end();
}

MProgressIndicatorBarView::MProgressIndicatorBarView(MProgressIndicator *controller) :
    MWidgetView(controller),
    d_ptr(new MProgressIndicatorBarViewPrivate)
{
    Q_D(MProgressIndicatorBarView);

    d->q_ptr = this;
    d->controller = controller;

    connect(controller, SIGNAL(visibleChanged()), this, SLOT(visibilityChangedSlot()));

    connect(controller, SIGNAL(displayEntered()), this, SLOT(resumeAnimation()));
    connect(controller, SIGNAL(displayExited()), this, SLOT(pauseAnimation()));
}


MProgressIndicatorBarView::~MProgressIndicatorBarView()
{
    delete d_ptr;
}

void MProgressIndicatorBarViewPrivate::updateBarPosition()
{
    Q_Q(MProgressIndicatorBarView);

    bool reverse = qApp->isRightToLeft();
    QRect r(q->rect().toRect());
    const qreal offset = (qreal)(q->model()->value() - q->model()->minimum()) / (qreal)(q->model()->maximum() - q->model()->minimum());

    if (offset > 0) {
        if (!reverse)
            r.moveRight(offset * r.width());
        else
            r.moveLeft((1 - offset) * r.width());

        if (q->style()->knownBarTexture())
            backgroundPainter->drawImage( r,barBody );
    }
}

void MProgressIndicatorBarView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    Q_D(MProgressIndicatorBarView);

    if(d->barBody.isNull()) {
        d->figureOutSizes();
        d->createMaskOnGeometry();
        d->resetBarComposition();
        d->setupBarBody();
    }

    if( d->previousValue > model()->value() ) {
        d->resetBarComposition();
    }

    d->previousValue = model()->value();

    d->updateBarPosition();

    foreach(const char * member, modifications) {
        if (member == MProgressIndicatorModel::UnknownDuration) {
            if (model()->unknownDuration()) {
                d->animate(d->controller->isVisible());
            }
        }
    }

    update();
}

void MProgressIndicatorBarView::setupModel()
{
    MWidgetView::setupModel();

    Q_D(MProgressIndicatorBarView);

    if (model()->unknownDuration()) {
        d->animate(d->controller->isVisible());
    }

    update();
}

void MProgressIndicatorBarViewPrivate::figureOutSizes()
{
    Q_Q(MProgressIndicatorBarView);

    q->style()->progressBarMask()->borders(&leftWidth,&rightWidth,&top,&bottom);

    rightEndRect = QRectF( QPointF(0.0,0.0), QSizeF(
            rightWidth,
            q->rect().height() ) );

    leftEndRect = QRectF( QPointF(0.0,0.0), QSizeF(
            leftWidth,
            q->rect().height() ) );
}

void MProgressIndicatorBarViewPrivate::setupBarBody()
{
    Q_Q(MProgressIndicatorBarView);

    barBody = QImage( q->rect().size().toSize(), QImage::Format_ARGB32 );
    barBody.fill(Qt::transparent);

    // draw the mask
    QPainter painter(&barBody);
    painter.drawImage(q->rect().toRect(), barMask);
    painter.end();

    const QPixmap* texture;
    if( q->model()->unknownDuration() )
        texture = q->style()->unknownBarTexture()->pixmap();
    else
        texture = q->style()->knownBarTexture()->pixmap();

    // paint the filling onto the mask, in slices
    painter.begin(&barBody);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    const QRect sourceRect( QPoint(0,0), q->style()->knownBarTexture()->pixmap()->size() );
    const int step = sourceRect.width();
    for(int i=0;i<q->rect().width();i+=step) {
        const QRectF nextRect(i,0,step,q->rect().height());
        if(textureTiled())
            painter.drawTiledPixmap( nextRect, *texture);
        else // stretched
            painter.drawPixmap( nextRect, *texture, sourceRect);
    }
    painter.end();

    // draw the right end
    rightEndImage = QImage( rightEndRect.size().toSize(), QImage::Format_ARGB32 );

    painter.begin(&rightEndImage);
    if(textureTiled())
        painter.drawTiledPixmap(rightEndRect.toRect(), *texture);
    else // stretched
        painter.drawPixmap(rightEndRect.toRect(),*texture);
    painter.end();

    painter.begin(&rightEndImage);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawImage( rightEndRect.toRect(), rightEndMask );
    painter.end();

    rightEnd = rightEndImage;

    // draw the left end
    leftEndImage = QImage( leftEndRect.size().toSize(), QImage::Format_ARGB32 );

    painter.begin(&leftEndImage);
    q->style()->knownBarTexture()->draw( leftEndRect.toRect(), &painter );
    painter.end();

    painter.begin(&leftEndImage);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawImage( leftEndRect.toRect(), leftEndMask );
    painter.end();

    leftEnd = leftEndImage;
}

void MProgressIndicatorBarViewPrivate::compositeBarForUnknownDuration()
{
    Q_Q(MProgressIndicatorBarView);

    QPainter painter;

    const qreal bodyWidth = barBody.size().width() + q->style()->unknownBarTexture()->pixmap()->size().width();
    QRectF udLeftEndRect( QPointF(0,0), leftEndRect.size() );
    QRectF udBodyRect( udLeftEndRect.topRight(), QSizeF(bodyWidth, q->rect().height()) );
    QRectF udRightEndRect( udBodyRect.topRight(), rightEndRect.size() );

    QRectF udBarRect( 0,0, leftWidth + bodyWidth + rightWidth, q->rect().height() );

    QPixmap* barImagePixmap = new QPixmap( udBarRect.size().toSize() );
    barImagePixmap->fill(Qt::transparent);

    painter.begin(barImagePixmap);
    painter.drawImage( udLeftEndRect, leftEndImage );
    painter.drawImage( udBodyRect.toRect(), barBody );
    painter.drawImage( udRightEndRect, rightEndImage );
    painter.end();

    scalableBarImage = new MScalableImage( barImagePixmap, leftWidth, rightWidth, top, bottom );
}

void MProgressIndicatorBarViewPrivate::resetBarComposition()
{
    Q_Q(MProgressIndicatorBarView);

    if( backgroundPainter->isActive() )
        backgroundPainter->end();

    barComposition = QPixmap(q->rect().size().toSize());
    barComposition.fill(Qt::transparent);

    if(backgroundPainter->begin(&barComposition)) {
        q->style()->progressBarBackground()->draw( q->rect().toRect(), backgroundPainter );
        backgroundPainter->setCompositionMode( QPainter::CompositionMode_SourceAtop );
    }
}

/*
 * creates a cache of animationCacheSize pixmaps to be drawn by drawContents in case width of unknown progress bar is 100%
 * cached images move into the scene from the left, thus single frames have width of bar + width of 1 tile
 * animation moves x times per tile
 *
 *   tile + bar
 *   +--+---------------------+
 *   |  |  -->    -->    -->  |
 *   +--+---------------------+
 */

void MProgressIndicatorBarViewPrivate::buildAnimationCache()
{
    Q_Q(MProgressIndicatorBarView);

    const int width_of_tile = q->style()->unknownBarTexture()->pixmap()->width();
    const QSize cachedPixmapSize( q->rect().size().toSize().width() + width_of_tile, q->rect().size().toSize().height() );
    const int slice_width = 1; //px

    animationCache.clear();
    QPainter painter;

    animationCacheSize = width_of_tile / slice_width;
    int slice_x = -width_of_tile;

    for(int i=0; i<animationCacheSize; i++)
    {
        QImage* cachedPixmap = new QImage(cachedPixmapSize,QImage::Format_ARGB32);
        cachedPixmap->fill(Qt::transparent);

        backgroundPainter->drawPixmap( slice_x += slice_width, 0, *scalableBarImage->pixmap() );

        painter.begin(cachedPixmap);
        painter.drawPixmap(0,0, barComposition);
        painter.end();

        animationCache.append(cachedPixmap);
    }
    setupAnimation();
}

void MProgressIndicatorBarViewPrivate::setupAnimation()
{
    Q_Q(MProgressIndicatorBarView);

    if(!q->model()->unknownDuration())
        return;

    // "position" is an index in animation cache
    qreal fps = 20; // ~ 10% CPU
    int interval = 1/fps*1000;
    timer->setInterval(interval);

    if(!timer->isActive()) {
        connect(timer, SIGNAL(timeout()), this, SLOT(setPosition()));
    }

}

void MProgressIndicatorBarView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const MProgressIndicatorBarView);

    if (style()->progressBarMask() && style()->progressBarBackground()) {

        bool reverse = qApp->isRightToLeft();

        QRect r(rect().toRect());
        style()->progressBarBackground()->draw(r, painter);

        int left, right;
        style()->progressBarMask()->borders(&left, &right, NULL, NULL);
        int minimumScalableWidth = left + right;

        if (!model()->unknownDuration()) {
            qreal offset = (qreal)(model()->value() - model()->minimum()) / (qreal)(model()->maximum() - model()->minimum());
            if (offset > 0) {
                if (!reverse) {
                    r.moveRight(offset * r.width());
                } else {
                    r.moveLeft((1 - offset) * r.width());
                }

                if (style()->knownBarTexture()) {
                    painter->drawPixmap( 0,0, d->barComposition );
                    if(!reverse) {
                        const QRect rightEndRect(QPoint(r.topRight().x(),0),d->rightEndRect.size().toSize());
                        painter->drawImage( rightEndRect, d->rightEnd );
                    } else {
                        const QRect leftEndRect(QPoint(r.topLeft().x()-d->leftEndRect.width(),0),d->leftEndRect.size().toSize());
                        painter->drawImage( leftEndRect, d->leftEnd );
                    }
                }
            }

        }
        // 100% width
        else if (d->fullWidth()) {
            if ((d->position >= 0) && (d->position < d->animationCacheSize))
                painter->drawImage( 0,0, *d->animationCache[d->position] );

        // need to draw in 1 or 2 parts, depending if the indicator element goes across the ends
        } else {
            qreal distance = d->position ;//* (qreal) r.width();

            if ((distance + style()->elementSize()) > r.width()) {
                // two draw calls
                QRect r2(r);
                if (!reverse) {
                    r.setLeft(distance);
                    r2.setRight(style()->elementSize() - r.width());
                } else {
                    r.setLeft(0);
                    r.setRight(r.width() - distance);
                    r2.setLeft(r2.width() - (style()->elementSize() - r.width()));
                }

                if (r.width() >= minimumScalableWidth)
                    d->scalableBarImage->draw( r, painter );
                if (r2.width() >= minimumScalableWidth)
                    d->scalableBarImage->draw( r2, painter );
            } else {
                // one draw call
                if (!reverse) {
                    r.setLeft(distance);
                    r.setRight(distance + style()->elementSize());
                    if (r.width() < minimumScalableWidth)
                        r.setRight(r.left() + minimumScalableWidth);
                } else {
                    r.setRight(r.width() - distance);
                    r.setLeft(r.right() - style()->elementSize());
                    if (r.width() < minimumScalableWidth)
                        r.setLeft(r.right() - minimumScalableWidth);
                }
                d->scalableBarImage->draw( r, painter );
            }
        }
    }
}

void MProgressIndicatorBarView::visibilityChangedSlot()
{
    Q_D(MProgressIndicatorBarView);

    d->animate(d->controller->isVisible());
}

void MProgressIndicatorBarView::resumeAnimation()
{
    Q_D(MProgressIndicatorBarView);

    d->animate(true);
}

void MProgressIndicatorBarView::pauseAnimation()
{
    Q_D(MProgressIndicatorBarView);

    d->animate(false);
}

void MProgressIndicatorBarView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MProgressIndicatorBarView);

    MWidgetView::resizeEvent(event);

    if (d->width != rect().width()) {
        d->width = rect().width();
    }

    d->setupAnimation();

    d->figureOutSizes();
    d->createMaskOnGeometry();
    d->resetBarComposition();
    d->setupBarBody();

    d->updateBarPosition();

    if(model()->unknownDuration()) {
        d->compositeBarForUnknownDuration();
        d->buildAnimationCache();
    }
}

void MProgressIndicatorBarView::changeEvent(QEvent *event)
{
    Q_D(MProgressIndicatorBarView);
    if (event->type() == QEvent::LayoutDirectionChange) {
        d->resetBarComposition();
    }
    MWidgetView::changeEvent(event);
}


// bind controller widget and view widget together by registration macro
M_REGISTER_VIEW_NEW(MProgressIndicatorBarView, MProgressIndicator)
