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
       animation(0),
       width(0),
       previousValue(0)
{
    backgroundPainter = new QPainter;
    scalableBarImage = new MScalableImage;
}


MProgressIndicatorBarViewPrivate::~MProgressIndicatorBarViewPrivate()
{
    delete backgroundPainter;
    delete scalableBarImage;
}

void MProgressIndicatorBarViewPrivate::setPosition(qreal pos)
{
    Q_Q(MProgressIndicatorBarView);

    position = pos;
    q->update();
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
        animation->start();
    } else {
        animation->stop();
    }
}

void MProgressIndicatorBarViewPrivate::createMaskOnGeometry()
{
    Q_Q(MProgressIndicatorBarView);

    const int height = q->rect().height();
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

    QPixmap canvas(q->rect().width(),height);
    canvas.fill(Qt::transparent);

    leftEndMask = QPixmap(leftWidth,height);
    leftEndMask.fill(Qt::transparent);

    rightEndMask = QPixmap(rightWidth,height);
    rightEndMask.fill(Qt::transparent);

    // first scale up and draw the entire mask image
    p.begin( &canvas );
    q->style()->maskImage()->draw(q->rect().toRect(), &p);
    p.end();

    // then cut out left end
    p.begin(&leftEndMask);
    p.drawPixmap( QRect(topLeft,leftEndMask.size()), canvas, QRect(topLeft,QSize(leftWidth,height)) );
    p.end();

    // and cut out right end
    p.begin(&rightEndMask);
    p.drawPixmap( QRect(topLeft,rightEndMask.size()), canvas, QRect(QPoint(q->rect().toRect().width()-rightWidth,0),QSize(rightWidth,height)) );
    p.end();

    barMask = QPixmap( QSize(q->style()->fillImage()->pixmap()->width(),height) );
    barMask.fill(Qt::transparent);

    // alpha blending is more correct in the middle
    const QPoint centerTop( q->rect().center().x() - barMask.size().width()/2, q->rect().y() );
    p.begin(&barMask);
    p.drawPixmap( QRect(QPoint(0,0),barMask.size()), canvas, QRect(centerTop, barMask.size()) );
    p.end();
}

MProgressIndicatorBarView::MProgressIndicatorBarView(MProgressIndicator *controller) :
    MWidgetView(controller),
    d_ptr(new MProgressIndicatorBarViewPrivate)
{
    Q_D(MProgressIndicatorBarView);

    d->q_ptr = this;
    d->controller = controller;

    d->animation = new QPropertyAnimation(d, "position", d);
    // "position" is a value between 0.0 and 1.0
    d->animation->setStartValue(0.0);
    d->animation->setEndValue(1.0);
    // the animation drives the unknownDuration-mode, so it loops forever
    // until the animation is stop()ed
    d->animation->setLoopCount(-1);

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

        if (q->style()->fillImage())
            backgroundPainter->drawPixmap( r,barBody );
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

    q->style()->maskImage()->borders(&leftWidth,&rightWidth,&top,&bottom);

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

    barBody = QPixmap( q->rect().size().toSize() );
    barBody.fill(Qt::transparent);

    q->style()->maskImage()->borders(&leftWidth,&rightWidth,&top,&bottom);

    // draw the mask
    QPainter painter(&barBody);
    painter.drawPixmap(q->rect().toRect(), barMask);
    painter.end();

    // paint the filling onto the mask, in slices
    painter.begin(&barBody);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    const QRect sourceRect( QPoint(0,0), q->style()->fillImage()->pixmap()->size() );
    const int step = sourceRect.width();
    for(int i=0;i<q->rect().width();i+=step) {
        const QRectF nextRect(i,0,step,q->rect().height());
        if(q->style()->fillImageTiled())
            painter.drawTiledPixmap( nextRect, *q->style()->fillImage()->pixmap());
        else // stretched
            painter.drawPixmap( nextRect, *q->style()->fillImage()->pixmap(), sourceRect);
    }
    painter.end();

    // draw the right end
    rightEndImage = QImage( rightEndRect.size().toSize(), QImage::Format_ARGB32 );
qCritical() << q->style()->fillImageTiled();
    painter.begin(&rightEndImage);
    if(q->style()->fillImageTiled())
        painter.drawTiledPixmap( rightEndRect.toRect(), *q->style()->fillImage()->pixmap() );
    else // stretched
        q->style()->fillImage()->draw( rightEndRect.toRect(), &painter );
    painter.end();

    painter.begin(&rightEndImage);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawPixmap( rightEndRect.toRect(), rightEndMask );
    painter.end();

    rightEnd = QPixmap::fromImage( rightEndImage );

    // draw the left end
    leftEndImage = QImage( leftEndRect.size().toSize(), QImage::Format_ARGB32 );

    painter.begin(&leftEndImage);
    q->style()->fillImage()->draw( leftEndRect.toRect(), &painter );
    painter.end();

    painter.begin(&leftEndImage);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawPixmap( leftEndRect.toRect(), leftEndMask );
    painter.end();

    leftEnd = QPixmap::fromImage(leftEndImage);
}

void MProgressIndicatorBarViewPrivate::compositeBarForUnknownDuration()
{
    Q_Q(MProgressIndicatorBarView);

    QPainter painter;

    const qreal bodyWidth = 100.0;
    QRectF udLeftEndRect( QPointF(0,0), leftEndRect.size() );
    QRectF udBodyRect( udLeftEndRect.topRight(), QSizeF(bodyWidth, q->rect().height()) );
    QRectF udRightEndRect( udBodyRect.topRight(), rightEndRect.size() );

    QRectF udBarRect( 0,0, leftWidth + bodyWidth + rightWidth, q->rect().height() );

    QPixmap* activeImagePixmap = new QPixmap( udBarRect.size().toSize() );
    activeImagePixmap->fill(Qt::transparent);

    painter.begin(activeImagePixmap);
    painter.drawImage( udLeftEndRect, leftEndImage );
    painter.drawPixmap( udBodyRect.toRect(), barBody );
    painter.drawImage( udRightEndRect, rightEndImage );
    painter.end();

    scalableBarImage = new MScalableImage( activeImagePixmap, leftWidth, rightWidth, top, bottom );
}

void MProgressIndicatorBarViewPrivate::resetBarComposition()
{
    Q_Q(MProgressIndicatorBarView);

    if( backgroundPainter->isActive() )
        backgroundPainter->end();

    barComposition = QPixmap(q->rect().size().toSize());
    barComposition.fill(Qt::transparent);

    if(backgroundPainter->begin(&barComposition)) {
        q->style()->inactiveImage()->draw( q->rect().toRect(), backgroundPainter );
        backgroundPainter->setCompositionMode( QPainter::CompositionMode_SourceAtop );
    }
}

void MProgressIndicatorBarView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const MProgressIndicatorBarView);

    if (style()->activeImage() && style()->inactiveImage()) {

        bool reverse = qApp->isRightToLeft();

        QRect r(rect().toRect());
        style()->inactiveImage()->draw(r, painter);

        int left, right;
        style()->activeImage()->borders(&left, &right, NULL, NULL);
        int minimumScalableWidth = left + right;

        if (!model()->unknownDuration()) {
            qreal offset = (qreal)(model()->value() - model()->minimum()) / (qreal)(model()->maximum() - model()->minimum());
            if (offset > 0) {
                if (!reverse) {
                    r.moveRight(offset * r.width());
                } else {
                    r.moveLeft((1 - offset) * r.width());
                }

                if (style()->fillImage()) {
                    painter->drawPixmap( 0,0, d->barComposition );
                    const int shift_left_to_avoid_flickering = 0;
                    const QRect rightEndRect(QPoint(r.topRight().x()-shift_left_to_avoid_flickering,0),d->rightEndRect.size().toSize());
                    painter->drawPixmap( rightEndRect, d->rightEnd );
                }
            }
        } else {
            qreal distance = d->position * (qreal) r.width();

            // need to draw in 1 or 2 parts, depending if the indicator element goes across the ends
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

    if (d->animation->state() == QPropertyAnimation::Paused)
        d->animation->resume();
}

void MProgressIndicatorBarView::pauseAnimation()
{
    Q_D(MProgressIndicatorBarView);

    if (d->animation->state() == QPropertyAnimation::Running)
        d->animation->pause();
}

void MProgressIndicatorBarView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MProgressIndicatorBarView);

    MWidgetView::resizeEvent(event);

    if (d->width != rect().width()) {
        d->width = rect().width();
        d->animation->setDuration(d->width * 1000 / style()->speed());
    }

    if(model()->unknownDuration()) {
        d->figureOutSizes();
        d->createMaskOnGeometry();
        d->resetBarComposition();
        d->setupBarBody();
        d->compositeBarForUnknownDuration();
    }
}

// bind controller widget and view widget together by registration macro
M_REGISTER_VIEW_NEW(MProgressIndicatorBarView, MProgressIndicator)
