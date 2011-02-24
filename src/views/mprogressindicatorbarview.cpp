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
#include "mprogressindicator_p.h"
#include "mscalableimage.h"
#include "mtheme.h"
#include "mdebug.h"

#define ANIMATION_SLOWDOWN_RATIO 4

MProgressIndicatorBarViewPrivate::MProgressIndicatorBarViewPrivate()
    :   controller(0),
        q_ptr(0),
        animationTimer(0),
        inSwitcher(false),
        animationCacheIndex(0)
{
    animationTimer = new QTimer(this);
}


MProgressIndicatorBarViewPrivate::~MProgressIndicatorBarViewPrivate()
{
    clearAnimationCache();
}

void MProgressIndicatorBarViewPrivate::drawBar(QPainter* painter) const
{
    Q_Q(const MProgressIndicatorBarView);

    const MProgressIndicatorStyle *s = static_cast<const MProgressIndicatorStyle *>(q->style().operator ->());

    bool reverse = qApp->isRightToLeft();
    QRectF viewRect(q->rect());

    if (s->progressBarBackground() &&
        s->progressBarBackground()->pixmap() &&
        !s->progressBarBackground()->pixmap()->isNull())
        s->progressBarBackground()->draw(q->rect().toRect(), painter);

    if (q->model()->unknownDuration()) {
        if (animationCacheIndex + s->elementSize() > viewRect.width()) {
            QRectF rect1(viewRect);
            QRectF rect2(viewRect);

            //rect1 will be closer to progress bar origin, rect2 further
            //total wdith of rect1 and rect2 has to be q->style()->elementSize()
            if (!reverse) {
                rect1.setLeft(viewRect.left());
                rect1.setWidth(s->elementSize() - (viewRect.right() - animationCacheIndex));

                rect2.setLeft(animationCacheIndex);
                rect2.setRight(viewRect.right());
            } else {
                rect1.setLeft(2 * viewRect.right() - (animationCacheIndex + s->elementSize()));
                rect1.setRight(viewRect.right());

                rect2.setLeft(viewRect.left());
                rect2.setRight(viewRect.right() - animationCacheIndex);
            }

            drawComposedRectangle(painter, rect1);
            drawComposedRectangle(painter, rect2);
        } else {
            if (!reverse)
                viewRect.moveLeft(animationCacheIndex);
            else
                viewRect.moveLeft(viewRect.width() - animationCacheIndex - s->elementSize());

            viewRect.setWidth(s->elementSize());

            drawComposedRectangle(painter, viewRect);
        }
    } else {
        qreal offset = 0;
        if (q->model()->maximum() != q->model()->minimum())
            offset = static_cast<qreal>(q->model()->value() - q->model()->minimum()) /
                     static_cast<qreal>(q->model()->maximum() - q->model()->minimum());
        if (offset > 0) {
            if (!reverse)
                viewRect.setWidth(offset * viewRect.width());
            else {
                viewRect.moveLeft((1 - offset) * viewRect.width());
                viewRect.setWidth(offset * viewRect.width());
            }

            drawComposedRectangle(painter, viewRect);
        }
    }
}

void MProgressIndicatorBarViewPrivate::clearBarImages()
{
    barBodyImage = QImage();
    leftEndImage = QImage();
    rightEndImage = QImage();
}

void MProgressIndicatorBarViewPrivate::clearAnimationCache()
{
    int actualCacheSize = animationCache.size();

    for(int i = 0; i < actualCacheSize; i++)
        delete animationCache[i];

    animationCache.clear();
}

const QImage* MProgressIndicatorBarViewPrivate::getCurrentCachedImage() const
{
    if ((animationCacheIndex < 0) || (animationCacheIndex > animationCache.size() - 1))
        return 0;

    return animationCache[animationCacheIndex];
}

bool MProgressIndicatorBarViewPrivate::fullWidth() const
{
    Q_Q(const MProgressIndicatorBarView);
    return (q->style()->unknownBarSize().width() >= q->rect().width());
}

bool MProgressIndicatorBarViewPrivate::barImagesCreated() const
{
    if (barBodyImage.isNull() || leftEndImage.isNull() || rightEndImage.isNull())
        return false;

    return true;
}

void MProgressIndicatorBarViewPrivate::animate(bool animate)
{
    Q_Q(MProgressIndicatorBarView);

    animate = (animate && q->model()->unknownDuration());
    if (animate)
        animationTimer->start();
    else
        animationTimer->stop();
}

void MProgressIndicatorBarViewPrivate::setupAnimation()
{
    Q_Q(MProgressIndicatorBarView);

    disconnect(animationTimer, SIGNAL(timeout()), this, SLOT(setAnimationCacheIndex()));
    animationTimer->stop();

    if(!q->model()->unknownDuration())
        return;

    // "position" is an index in animation cache
    qreal fps = static_cast<qreal>(q->style()->refreshRate());
    if (fps <= 0)
       fps = 20;
    int interval = (1 / fps) * 1000;
    if (inSwitcher)
        animationTimer->setInterval(interval * ANIMATION_SLOWDOWN_RATIO);
    else
        animationTimer->setInterval(interval);

    connect(animationTimer, SIGNAL(timeout()), this, SLOT(setAnimationCacheIndex()));
    animationTimer->start();
}

void MProgressIndicatorBarViewPrivate::setupBarImages()
{
    Q_Q(MProgressIndicatorBarView);

    createBarImages();
    q->update(q->rect());
}

const MWindow* MProgressIndicatorBarViewPrivate::getMWindow()
{
    QGraphicsScene *scene = controller->scene();

    if (!scene || scene->views().isEmpty())
        return 0;

    return qobject_cast<MWindow*>(scene->views().at(0));
}

void MProgressIndicatorBarViewPrivate::setAnimationCacheIndex()
{
    Q_Q(MProgressIndicatorBarView);

    if (q->model()->unknownDuration()) {
        if (fullWidth()) {
            if (animationCache.isEmpty())
                createAnimationCache();
        } else {
            if (!barImagesCreated())
                createBarImages();
        }
    }

    animationCacheIndex++;
    if(fullWidth() && animationCacheIndex > animationCache.size() - 1)
        animationCacheIndex = 0;
    else if (!fullWidth() && animationCacheIndex >= q->rect().width())
        animationCacheIndex = 0;

    q->update(q->rect());
}

void MProgressIndicatorBarViewPrivate::switcherEntered()
{
    inSwitcher = true;
    setupAnimation();
}

void MProgressIndicatorBarViewPrivate::switcherExited()
{
    inSwitcher = false;
    setupAnimation();
}

void MProgressIndicatorBarViewPrivate::createBarImages()
{
    Q_Q(MProgressIndicatorBarView);

    if (barImagesCreated())
        return;

    const MProgressIndicatorStyle *s = static_cast<const MProgressIndicatorStyle *>(q->style().operator ->());

    if (!s->progressBarMask() ||
        !s->progressBarMask()->pixmap() ||
        s->progressBarMask()->pixmap()->isNull())
        return;

    if (q->model()->unknownDuration()) {
        if (!s->unknownBarTexture() ||
            !s->unknownBarTexture()->pixmap() ||
            s->unknownBarTexture()->pixmap()->isNull())
            return;
    } else {
        if (!s->knownBarTexture() ||
            !s->knownBarTexture()->pixmap() ||
            s->knownBarTexture()->pixmap()->isNull())
            return;
    }

    QRect viewRect(q->rect().toRect());
    if (viewRect.isEmpty())
        return;

    QPainter painter;

    QImage leftEndMask;
    QImage rightEndMask;
    QImage barMask;

    int left, right, top, bottom;
    s->progressBarMask()->borders(&left, &right, &top, &bottom);

    QRect leftEndRect(QPoint(0, 0), QSize(left, q->rect().toRect().height()));
    QRect rightEndRect(QPoint(0, 0), QSize(right, q->rect().toRect().height()));

    //
    //  leftEnd      barMask      rightEnd
    //  +-----+-------+---+--------+-----+
    //  |     |       |///|        |     |
    //  |     |       |///|        |     |
    //  |     |       |///|        |     |
    //  +-----+-------+---+--------+-----+
    //

    QImage canvas;
    if (q->model()->unknownDuration())
        canvas = QImage(viewRect.width() + s->unknownBarTexture()->pixmap()->width(),
                    viewRect.height(),
                    QImage::Format_ARGB32);
    else
        canvas = QImage(viewRect.width() + s->knownBarTexture()->pixmap()->width(),
                    viewRect.height(), QImage::Format_ARGB32);

    canvas.fill(Qt::transparent);

    leftEndMask = QImage(left, viewRect.height(), QImage::Format_ARGB32);
    leftEndMask.fill(Qt::transparent);

    rightEndMask = QImage(right, viewRect.height(), QImage::Format_ARGB32);
    rightEndMask.fill(Qt::transparent);

    if (painter.begin(&canvas)) {
        s->progressBarMask()->draw(canvas.rect(), &painter);
        painter.end();
    }

    // then cut out left end
    if (painter.begin(&leftEndMask)) {
        painter.drawImage(QRect(QPoint(0, 0),
                            leftEndMask.size()),
                            canvas,
                            QRect(QPoint(0, 0),
                            QSize(left, canvas.rect().height())));
        painter.end();
    }

    // and cut out right end
    if (painter.begin(&rightEndMask)) {
        painter.drawImage(QRect(QPoint(0, 0),
                            rightEndMask.size()),
                            canvas,
                            QRect(QPoint(canvas.rect().width()-right, 0),
                            QSize(right, canvas.rect().height())));
        painter.end();
    }

    if (q->model()->unknownDuration())
        barMask = QImage(QSize(s->unknownBarTexture()->pixmap()->width(),
                        canvas.rect().height()),
                        QImage::Format_ARGB32);
    else
        barMask = QImage(QSize(s->knownBarTexture()->pixmap()->width(),
                        canvas.rect().height()),
                        QImage::Format_ARGB32);

    barMask.fill(Qt::transparent);

    // alpha blending is more correct in the middle
    QPoint centerTop(canvas.rect().center().x() - barMask.size().width()/2, canvas.rect().y());
    if (painter.begin(&barMask)) {
        painter.drawImage(QRect(QPoint(0, 0), barMask.size()), canvas, QRect(centerTop, barMask.size()));
        painter.end();
    }

    barBodyImage = QImage(canvas.rect().size(), QImage::Format_ARGB32);
    barBodyImage.fill(Qt::transparent);

    // draw the mask
    if (painter.begin(&barBodyImage)) {
        painter.drawImage(canvas.rect(), barMask);
        painter.end();
    }

    // paint the filling onto the mask, in slices
    if (painter.begin(&barBodyImage)) {
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

        int step;
        if (q->model()->unknownDuration())
            step = s->unknownBarTexture()->pixmap()->width();
        else
            step = s->knownBarTexture()->pixmap()->width();

        if (step > 0) {
            for (int i = 0; i < canvas.rect().width(); i += step)
                drawTexture(&painter, QRectF(i, 0, step, q->rect().height()));
        }
        painter.end();
    }

    // draw the right end
    rightEndImage = QImage(rightEndRect.size(), QImage::Format_ARGB32);
    rightEndImage.fill(0);

    if (painter.begin(&rightEndImage)) {
        drawTexture(&painter, rightEndRect);

        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        painter.drawImage(rightEndRect, rightEndMask);
        painter.end();
    }

    // draw the left end
    leftEndImage = QImage(leftEndRect.size(), QImage::Format_ARGB32);
    leftEndImage.fill(0);

    if (painter.begin(&leftEndImage)) {
        drawTexture(&painter, leftEndRect);

        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        painter.drawImage(leftEndRect, leftEndMask);
        painter.end();
    }
}

 // creates a cache of animationCacheSize pixmaps to be drawn by drawContents
 // in case width of unknown progress bar is 100% cached images move into the
 // scene from the left, thus single frames have width of bar + width of 1 tile
 // animation moves x times per tile
 //
 //   tile + bar
 //   +--+---------------------+
 //   |  |  -->    -->    -->  |
 //   +--+---------------------+
void MProgressIndicatorBarViewPrivate::createAnimationCache()
{
    Q_Q(MProgressIndicatorBarView);

    const MProgressIndicatorStyle *s = static_cast<const MProgressIndicatorStyle *>(q->style().operator ->());

    bool reverse = qApp->isRightToLeft();
    QRect viewRect = q->rect().toRect();

    if (!s->progressBarBackground() ||
        !s->progressBarBackground()->pixmap() ||
        s->progressBarBackground()->pixmap()->isNull())
       return;

    if (barBodyImage.isNull())
        createBarImages();

    if (barBodyImage.isNull())
       return;

    int cacheSize;
    if (q->model()->unknownDuration())
        cacheSize = s->unknownBarTexture()->pixmap()->width();
    else
        cacheSize = s->knownBarTexture()->pixmap()->width();

    QPainter painter;

    int sliceWidth = 1;
    int sliceX;
    if (!reverse)
        sliceX = -cacheSize;
    else
        sliceX = 0;

    for (int i = 0; i < cacheSize; i += sliceWidth) {
        QImage* cachedImage = new QImage(viewRect.size(), QImage::Format_ARGB32);
        cachedImage->fill(Qt::transparent);

        if (painter.begin(cachedImage)) {
            s->progressBarBackground()->draw(viewRect, &painter);
            painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
            painter.drawImage(sliceX, 0, barBodyImage);
            painter.end();
        }

        if (!reverse)
            sliceX += sliceWidth;
        else
            sliceX -= sliceWidth;

        animationCache.append(cachedImage);
    }
}

void MProgressIndicatorBarViewPrivate::drawComposedRectangle(QPainter* painter, const QRectF& rect) const
{
    if (!barImagesCreated())
        return;

    QRectF drawnRect(rect);

    drawnRect.setLeft(drawnRect.left() + leftEndImage.width());
    drawnRect.setRight(drawnRect.right() - rightEndImage.width());

    if (drawnRect.width() >= 0) {
        painter->drawImage(drawnRect, barBodyImage, QRectF(QPointF(0, 0), drawnRect.size()));
        painter->drawImage(QRectF(QPointF(drawnRect.topLeft().x() - leftEndImage.width(), 0),
                            leftEndImage.size()),
                            leftEndImage);
        painter->drawImage(QRectF(drawnRect.topRight(), rightEndImage.size()), rightEndImage);
    }
}

void MProgressIndicatorBarViewPrivate::drawTexture(QPainter* painter, const QRectF& rect) const
{
    Q_Q(const MProgressIndicatorBarView);

    const MProgressIndicatorStyle *s = static_cast<const MProgressIndicatorStyle *>(q->style().operator ->());

    if (q->model()->unknownDuration()) {
       if (s->unknownBarTextureTiled())
           painter->drawTiledPixmap(rect, *s->unknownBarTexture()->pixmap());
       else
           painter->drawPixmap(rect,
                                *s->unknownBarTexture()->pixmap(),
                                QRect(QPoint(0,0),
                                s->unknownBarTexture()->pixmap()->size()));
    } else {
       if (s->knownBarTextureTiled())
          painter->drawTiledPixmap(rect, *s->knownBarTexture()->pixmap());
       else
          painter->drawPixmap(rect,
                                *s->knownBarTexture()->pixmap(),
                                QRect(QPoint(0,0),
                                s->knownBarTexture()->pixmap()->size()));
    }
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

void MProgressIndicatorBarView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    Q_D(MProgressIndicatorBarView);

    foreach(const char * member, modifications) {
        if (member == MProgressIndicatorModel::UnknownDuration) {
            d->clearBarImages();
            d->clearAnimationCache();
            d->setupAnimation();
        } else {
            if (model()->unknownDuration())
                d->animate(d->controller->isVisible());
            else {
                if (!d->barImagesCreated())
                    d->setupBarImages();
            }
        }
    }

    update();
}

void MProgressIndicatorBarView::setupModel()
{
    MWidgetView::setupModel();

    Q_D(MProgressIndicatorBarView);

    d->clearAnimationCache();

    if (model()->unknownDuration())
        d->setupAnimation();
    else {
        if (!d->barImagesCreated())
            d->setupBarImages();
    }

    update();
}

void MProgressIndicatorBarView::applyStyle()
{
    MWidgetView::applyStyle();

    Q_D(MProgressIndicatorBarView);

    d->clearBarImages();
    d->animationCache.clear();

    if (model()->unknownDuration())
        d->setupAnimation();
    else
        d->setupBarImages();

    update();
}

void MProgressIndicatorBarView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const MProgressIndicatorBarView);

    if (model()->unknownDuration() && d->fullWidth()) {
        const QImage* cachedImage = d->getCurrentCachedImage();
        if (cachedImage && !cachedImage->isNull())
            painter->drawImage(0, 0, *cachedImage);
    } else
        d->drawBar(painter);
}

void MProgressIndicatorBarView::visibilityChangedSlot()
{
    Q_D(MProgressIndicatorBarView);

    d->animate(d->controller->isVisible());
}

void MProgressIndicatorBarView::resumeAnimation()
{
    Q_D(MProgressIndicatorBarView);

    const MWindow *mWindow = d->getMWindow();
    if (mWindow) {
        d->connect(mWindow, SIGNAL(switcherEntered()), SLOT(switcherEntered()), Qt::UniqueConnection);
        d->connect(mWindow, SIGNAL(switcherExited()), SLOT(switcherExited()), Qt::UniqueConnection);
        if (mWindow->isInSwitcher()) {
            d->switcherEntered();
        }
    }

    d->animate(true);
}

void MProgressIndicatorBarView::pauseAnimation()
{
    Q_D(MProgressIndicatorBarView);

    d->inSwitcher = false;
    d->animate(false);
}

void MProgressIndicatorBarView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MProgressIndicatorBarView);

    MWidgetView::resizeEvent(event);

    d->clearBarImages();
    d->clearAnimationCache();

    if (model()->unknownDuration())
        d->setupAnimation();
    else
        d->setupBarImages();
}

void MProgressIndicatorBarView::changeEvent(QEvent *event)
{
    Q_D(MProgressIndicatorBarView);

    if (event->type() == QEvent::LayoutDirectionChange) {
        d->clearBarImages();
        d->clearAnimationCache();

        if (model()->unknownDuration())
            d->setupAnimation();
        else
            d->setupBarImages();
    }

    MWidgetView::changeEvent(event);
}

// bind controller widget and view widget together by registration macro
M_REGISTER_VIEW_NEW(MProgressIndicatorBarView, MProgressIndicator)
