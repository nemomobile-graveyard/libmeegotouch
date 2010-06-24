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
#include <QTimeLine>
#include <QTimer>
#include <QGraphicsSceneResizeEvent>
#include <QBitmap>
#include <math.h>

#include "mspinnerview.h"
#include "mspinnerview_p.h"

#include "mprogressindicator.h"
#include "mprogressindicator_p.h"
#include "mviewcreator.h"

#include "mtheme.h"
#include "mscalableimage.h"
#include "mdebug.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int SpinnerRefreshRate = 30;

MSpinnerViewPrivate::MSpinnerViewPrivate()
    :  q_ptr(0),
       controller(0),
       pieImage(0),
       position(0),
       elapsed(0),
       timer(0)
{
}


MSpinnerViewPrivate::~MSpinnerViewPrivate()
{
    delete timer;
}


MSpinnerView::MSpinnerView(MProgressIndicator *controller) :
    MWidgetView(controller),
    d_ptr(new MSpinnerViewPrivate)
{
    Q_D(MSpinnerView);
    d->q_ptr = this;
    d->controller = controller;

    d->bgRect = QRect();
    d->fgRect = QRect();
    d->pieImage = new QPixmap();

    connect(controller, SIGNAL(visibleChanged()), this, SLOT(visibilityChanged()));
}


MSpinnerView::~MSpinnerView()
{
    delete d_ptr;
}

void MSpinnerViewPrivate::animationTimeout()
{
    Q_Q(MSpinnerView);

    if (q->model()->unknownDuration()) {
        //calculate interval in secs and add it to elapsed time
        qreal interval = static_cast<qreal>(timer->interval() / 1000.0);
        elapsed += interval;

        //calculate how many steps we should take
        int steps = static_cast<int>(elapsed * (qreal) q->style()->speed());
        if (steps > 0) {
            //subtract the amount we will step from the elapsed time
            elapsed -= steps * (1.0 / (qreal) q->style()->speed());
            //and perform the stepping
            position = (position + steps) % 360;
            //redraw
            q->update();
        }
    }
}

void MSpinnerView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    Q_D(MSpinnerView);

    foreach(const char * member, modifications) {
        if (member == MProgressIndicatorModel::UnknownDuration) {
            if (model()->unknownDuration()) {
                if (!d->timer) {
                    d->timer = new QTimer(this);
                    connect(d->timer, SIGNAL(timeout()), this, SLOT(animationTimeout()));
                }
                if (d->controller->isVisible())
                    d->timer->start(SpinnerRefreshRate);
                } else {
                    delete d->timer;
                    d->timer = NULL;
                }
            }
        }

    applyStyle();
    update();
}


void MSpinnerView::setupModel()
{
    MWidgetView::setupModel();

    Q_D(MSpinnerView);

    if (model()->unknownDuration()) {
        if (!d->timer) {
            d->timer = new QTimer(this);
            connect(d->timer, SIGNAL(timeout()), this, SLOT(animationTimeout()));
        }
        if (d->controller->isVisible())
            d->timer->start(SpinnerRefreshRate);
    } else {
        delete d->timer;
        d->timer = NULL;
    }

   update();
}


void MSpinnerView::calculateSizes()
{
    Q_D(MSpinnerView);
    QSize s;
    //calculating sizes for background
    if ( rect().toRect().height() >= rect().toRect().width() ) {
        s = QSize(rect().toRect().width(), rect().toRect().width());
    } else {
        s = QSize(rect().toRect().height(), rect().toRect().height());
    }

   QPoint bgDrawPoint(size().width() * 0.5 - s.width() * 0.5, size().height() * 0.5 - s.height() * 0.5 );

   d->bgRect = QRect(bgDrawPoint, s);

   //calculating foreground size:
   d->fgRect = QRect(
           QPoint((d->bgRect.topLeft().x() + style()->progressStripSize()), (d->bgRect.topLeft().y() + style()->progressStripSize())),
           QPoint((d->bgRect.bottomRight().x() - style()->progressStripSize()), (d->bgRect.bottomRight().y() - style()->progressStripSize())));

  *d->pieImage = QPixmap(d->bgRect.size());
  }

void MSpinnerView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
   Q_D(const MSpinnerView);
   Q_UNUSED(option);
   Q_UNUSED(painter);

   style()->bgImage()->draw(d->bgRect, painter);
}

void MSpinnerView::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
  Q_UNUSED(option);
  Q_UNUSED(painter);

  /*previously drawing foreground was here, but it is drawing it incorrectly for some reason
  drawing foreground is moved to drawContents temporary*/
}

QRect centerScale( QRect const & r, double s )
{
    int w = r.width() * s;
    int h = r.height() * s;
    int x = r.x() + (r.width() - w)/2;
    int y = r.y() + (r.height() - h)/2;
    return QRect(x, y, w, h);
}

void MSpinnerView::createPieImage( int startAngle, int endAngle, QSize const & size, QPixmap const & image ) const
{
    Q_D(const MSpinnerView);
    d->pieImage->rect().setSize( size );
    d->pieImage->fill( Qt::transparent);

    QPainter piePainter( d->pieImage );
    piePainter.setBrush( QBrush(image) );
    piePainter.setPen( QPen(Qt::transparent) );

    // Qt starts at 3:00 and goes CCW, we provide values starting at 12:00 and going CW
    piePainter.drawPie( centerScale(QRect(QPoint(),size), 1.5),
                                         (90-endAngle)*16,
                                         (endAngle-startAngle)*16 );
}

void MSpinnerView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    Q_D(const MSpinnerView);

    // Temporary: these should be made as style parameters
    static int const minScale = 75; // scale used for minimum size when growing/shrinking
    static int const maxScale = 100; // maybe not needed as style
    static int const sweepAngle = 90; // sweep angle for unknown duration
    static int const resolution = 100; // number of divisions in circle for unknown duration mode

    // calculated values input into rendering
    double startAngle = 0.0;
    double endAngle = 0.0;
    double scale = 1.0;

    // Calculate values depending on mode
    if ( model()->unknownDuration() ) {
        startAngle = d->position * 360/resolution;
        endAngle = startAngle + sweepAngle;
        scale = 1.0;
    }
    else {
        startAngle = 0.0;
        endAngle =  360*(model()->value() - model()->minimum()) / (model()->maximum() - model()->minimum());

        double growAngle = 1; //can be adjusted if grow/shrink animation for not uknown duration required
        double angleIncrement = (maxScale-minScale)/growAngle; // conversion from angle to scale

        scale = endAngle < growAngle ? minScale + endAngle * angleIncrement :
                      endAngle > 360-growAngle ? minScale + (360-endAngle) * angleIncrement : maxScale;
        scale /= 100.0;
    }

    // Render the image
    createPieImage( startAngle, endAngle, d->bgRect.size(), *style()->progressImage()->pixmap() );
    d->pieImage->setMask( style()->maskImage()->pixmap()->createHeuristicMask() );
    painter->drawPixmap( scale > .99 ? d->bgRect : centerScale(d->bgRect, scale), *d->pieImage, d->pieImage->rect() );

    //drawing foreground here, because there's something wrong with drawForeground()
    style()->fgImage()->draw( d->fgRect, painter);
}

void MSpinnerView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MWidgetView::resizeEvent(event);
    calculateSizes();
}

void MSpinnerView::applyStyle()
{
     MWidgetView::applyStyle();
     calculateSizes();
}


void MSpinnerViewPrivate::visibilityChanged()
{
    Q_Q(MSpinnerView);
    if (timer) {
        if (controller->isVisible()) {
            timer->start(SpinnerRefreshRate);
        } else {
            timer->stop();
        }
    }
    q->calculateSizes();
}

#include "moc_mspinnerview.cpp"

// bind controller widget and view widget together by registration macro
M_REGISTER_VIEW_NEW(MSpinnerView, MProgressIndicator)
