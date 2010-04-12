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

#include "qtmaemo6scrollbareventfilter.h"
#include "qtmaemo6style_p.h"
#include "qtmaemo6windowdecoration.h"

#include <QWidget>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QApplication>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QPaintEvent>
#include <QDebug>

#include <mapplicationpagestyle.h>
#include <mpositionindicatorstyle.h>
#include <mwidgetstyle.h>
#include <mwidgetfadeinanimationstyle.h>
#include <mwidgetfadeoutanimationstyle.h>
#include <MScalableImage>

AbstractScrollAreaThumbView::AbstractScrollAreaThumbView(QWidget *parent /*= NULL*/)
    : QLabel(parent)
{
    setVisible(false);
}

void AbstractScrollAreaThumbView::paintEvent(QPaintEvent *event)
{
    if (pixmap()) {

        QPixmap pix = *pixmap();

        QPainter pixPainter(&pix);

        QPainterPath grayedOutRegion;
        grayedOutRegion.setFillRule(Qt::OddEvenFill);
        grayedOutRegion.addRect(QRect(QPoint(0, 0), pixmap()->size()));
        grayedOutRegion.addRect(m_visibleRect);

        pixPainter.setPen(Qt::NoPen);
        pixPainter.setBrush(QBrush(QColor(255, 255, 255, 128)));
        pixPainter.drawPath(grayedOutRegion);

        pixPainter.setPen(QPen(Qt::black, 1.0));
        pixPainter.setBrush(Qt::NoBrush);
        pixPainter.drawRect(m_visibleRect);
        pixPainter.end();

        pix = setPixmapOpacity(pix, m_opacity);

        QPainter painter(this);
        painter.drawPixmap(event->rect(), pix);
    }
}


QtMaemo6ScrollBarEventFilter::QtMaemo6ScrollBarEventFilter(QObject *parent) :
    QObject(parent),
    m_scrollBarsAlwaysVisible(false),
    m_scrollAreaThumbnailMaxSize(200),
    m_scrollAreaThumbnailOffset(20),
    m_scrollAreaThumbnailBorder(3),
    m_showScrollAreaThumbnailFactor(0.2)
{}

QtMaemo6ScrollBarEventFilter::~QtMaemo6ScrollBarEventFilter()
{
}

void QtMaemo6ScrollBarEventFilter::enableOn(QObject *o)
{
    o->installEventFilter(this);

    if (QAbstractScrollArea *abstractScrollArea = qobject_cast<QAbstractScrollArea *>(o)) {
#ifdef SHOW_SCROLLING_THUMBNAIL
        if (qobject_cast<QtMaemo6WindowDecoration *>(abstractScrollArea->parent())) {
            AbstractScrollAreaThumbView *scrollAreaThumbView = new AbstractScrollAreaThumbView(abstractScrollArea);
            scrollAreaThumbView->setObjectName(SCROLLAREATHUMBVIEW);
            scrollAreaThumbView->setOpacity(0);
        }
#endif
        //FIXME: find a more proper solution without manipulating the widget's properties
        // turn the scrollbars off for ScrollArea, they are handled manually by the style
        abstractScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        abstractScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    if (QScrollBar *scrollBar = qobject_cast<QScrollBar *>(o)) {
        //FIXME: using the signals from scrollbar is not the best solution, because
        // they maybe disconnected by the user
        connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarValueChanged()));
    }
}

bool QtMaemo6ScrollBarEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
        /* this is the implementation for showing the scrollbars without using the signals
           from the scrollbars
           this piece of code is kept intentionally, because it may be useful when not using
           the signals from the scrollbar to recognize that scrolling is in progress
        case QEvent::Move: {
            if(!scrollBarsAlwaysVisible()) {
                QMoveEvent* moveEvent = static_cast<QMoveEvent*>(event);
                if(QWidget* widget = qobject_cast<QWidget*>(obj)) {
                    QWidget* viewport = widget->parentWidget();
                    QAbstractScrollArea* scrollArea = qobject_cast<QAbstractScrollArea*>(viewport->parent());
                    if(scrollArea) {
                        generateScrollAreaThumb(scrollArea);
                        fadeInOutAnimation(scrollArea->findChild<QLabel*>(SCROLLAREATHUMBVIEW), "opacity");
                        if(scrollArea->horizontalScrollBar() && moveEvent->oldPos().x() != moveEvent->pos().x()) {
                            fadeInOutAnimation(scrollArea->horizontalScrollBar(), WIDGET_OPACITY);
                        }
                        if(scrollArea->verticalScrollBar() && moveEvent->oldPos().y() != moveEvent->pos().y()) {
                            fadeInOutAnimation(scrollArea->verticalScrollBar(), WIDGET_OPACITY);
                        }
                    }
                }
            }
        }
        break;
        */
    case QEvent::Resize: {
        if (QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea *>(obj)) {
            if (QScrollBar *scrollBar = scrollArea->horizontalScrollBar()) {
                //Moved the scrollBar init stuff into resizeEvent because
                // in QStyle::polish are no sizes set, so there can't be made
                // a decision about the need to show a scrollbar
                scrollBar->setAttribute(Qt::WA_OpaquePaintEvent, false);
                scrollBar->setParent(scrollArea);
                setScrollBarVisibility(scrollBar);

                scrollBar->setGeometry(QRect(scrollArea->contentsRect().left(),
                                             scrollArea->contentsRect().bottom() - scrollBar->sizeHint().height(),
                                             scrollArea->contentsRect().width(),
                                             scrollBar->sizeHint().height()));
            }
            if (QScrollBar *scrollBar = scrollArea->verticalScrollBar()) {
                //Moved the scrollBar init stuff into resizeEvent beacause
                // in QStyle::polish are no sizes set, so there can't be made
                // a decision about the need to show a scrollbar
                scrollBar->setAttribute(Qt::WA_OpaquePaintEvent, false);
                scrollBar->setParent(scrollArea);
                setScrollBarVisibility(scrollBar);

                if (qApp->isRightToLeft()) {
                    scrollBar->setGeometry(QRect(scrollArea->contentsRect().left(),
                                                 scrollArea->contentsRect().top(),
                                                 scrollBar->sizeHint().width(),
                                                 scrollArea->contentsRect().height()));
                } else {
                    scrollBar->setGeometry(QRect(scrollArea->contentsRect().right() - scrollBar->sizeHint().width(),
                                                 scrollArea->contentsRect().top(),
                                                 scrollBar->sizeHint().width(),
                                                 scrollArea->contentsRect().height()));
                }
            }
#ifdef SHOW_SCROLLING_THUMBNAIL
            if (QLabel *scrollAreaThumbView = scrollArea->findChild<QLabel *>(SCROLLAREATHUMBVIEW)) {
                setScrollAreaThumbGeometry(scrollAreaThumbView);
            }
#endif
        }
    }
    break;
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}

void QtMaemo6ScrollBarEventFilter::setScrollBarVisibility(QScrollBar *scrollBar)
{
    if (!scrollBarsAlwaysVisible()) {
        scrollBar->setProperty(WIDGET_OPACITY, 0.0);
    } else {
        //set the scrollBar only visible if there is something to scroll
        bool isValidScrollRange = scrollBar->minimum() < scrollBar->maximum();
        scrollBar->setVisible(isValidScrollRange);
    }
}


void QtMaemo6ScrollBarEventFilter::generateScrollAreaThumb(QAbstractScrollArea *scrollArea,
        bool forceUpdate /*= false*/)
{
    //surpress unused warning while this is inactive
    Q_UNUSED(scrollArea);
    Q_UNUSED(forceUpdate);

#ifdef SHOW_SCROLLING_THUMBNAIL
    if (AbstractScrollAreaThumbView *scrollAreaThumbView =
                scrollArea->findChild<AbstractScrollAreaThumbView *>(SCROLLAREATHUMBVIEW)) {
        if (scrollAreaThumbView) {
            QWidget *vp = scrollArea->viewport();

            double scaleFactor;
            double longSide = qMax(vp->childrenRect().width(), vp->childrenRect().height());

            if (0.0 != longSide) {
                scaleFactor = m_scrollAreaThumbnailMaxSize / longSide;

                if (!scrollAreaThumbView->pixmap() || forceUpdate) {
                    qDebug() << "generating Pixmap";

                    QPixmap pixmap(vp->childrenRect().size() * scaleFactor + QSize(m_scrollAreaThumbnailBorder * 2, m_scrollAreaThumbnailBorder * 2));
                    pixmap.fill(Qt::white);
                    QPainter p(&pixmap);

                    const MApplicationPageStyle *style =
                        static_cast<const MApplicationPageStyle *>(
                            QtMaemo6StylePrivate::mStyle(QStyle::State_Active, "MApplicationPageStyle", ""));

                    p.drawPixmap(pixmap.rect(), *style->backgroundImage()->pixmap());

                    //Drawing the border
                    p.setPen(QPen(Qt::black, 1.0));
                    p.drawRect(QRect(QPoint(0, 0), pixmap.size() - QSize(1, 1)));

                    p.translate(3, 3);
                    p.scale(scaleFactor, scaleFactor);
                    vp->render(&p, QPoint(), vp->childrenRect());

                    scrollAreaThumbView->setPixmap(pixmap);
                }

                QRect visibleRect = QRect(-vp->childrenRect().topLeft(), vp->rect().size());

                //scale visibleRect manually to fit the thumbnailview
                scrollAreaThumbView->setVisibleRect(
                    QRect(visibleRect.topLeft() * scaleFactor, visibleRect.size() * scaleFactor - QSize(1, 1)).translated(3, 3));

                setScrollAreaThumbGeometry(scrollAreaThumbView);
            }
        }
    }
#endif
}

void QtMaemo6ScrollBarEventFilter::cleanUpTimerMap()
{
    if (QAbstractAnimation *animation = qobject_cast<QAbstractAnimation *>(sender())) {
        //the event comes from the fadeOut-animation, so we need to get the group
        if ((animation = qobject_cast<QAbstractAnimation *>(animation->parent()))) {
            foreach(const QWidget * w, m_pendingAnimations.keys()) {
                if (m_pendingAnimations.value(w) == animation) {
                    delete m_pendingAnimations.take(w);
                }
            }
        }
    }
}

void QtMaemo6ScrollBarEventFilter::setScrollAreaThumbGeometry(QLabel *label)
{
    if (label->pixmap()) {
        if (qApp->isRightToLeft()) {
            label->setGeometry(m_scrollAreaThumbnailOffset, m_scrollAreaThumbnailOffset,
                               label->pixmap()->width(), label->pixmap()->height());
        } else {
            label->setGeometry(label->parentWidget()->width() - (label->pixmap()->width() + m_scrollAreaThumbnailOffset),
                               m_scrollAreaThumbnailOffset, label->pixmap()->width(), label->pixmap()->height());
        }
    }
}

void QtMaemo6ScrollBarEventFilter::scrollBarValueChanged()
{
    if (QScrollBar *scrollBar = qobject_cast<QScrollBar *>(sender())) {
        if (QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea *>(scrollBar->parent())) {
            int scrollBarRange = scrollBar->maximum() - scrollBar->minimum();

            double factor = 0;
            if (scrollBar->orientation() == Qt::Horizontal)
                factor = scrollBarRange / (double)scrollArea->viewport()->childrenRect().size().width();
            else
                factor = scrollBarRange / (double)scrollArea->viewport()->childrenRect().size().height();

            //only show the scrollArea thumbnail if the contents have enough amount of scrolling
            if (factor > m_showScrollAreaThumbnailFactor) {
                //if a new animation is started force generating a new thumbnail
                bool forceThumbnailUpdate = !m_pendingAnimations.contains(scrollBar);
                generateScrollAreaThumb(scrollArea, forceThumbnailUpdate);

                fadeInOutAnimation(scrollArea->findChild<QLabel *>(SCROLLAREATHUMBVIEW), "opacity");
            }

            //only show the scrollBar if it has a valid range
            if (scrollBarRange)
                fadeInOutAnimation(scrollBar, WIDGET_OPACITY);
        }
    }
}

void QtMaemo6ScrollBarEventFilter::fadeInOutAnimation(QWidget *w, const char *property)
{
    if (!w || !property)
        return;

    if (m_pendingAnimations.contains(w)) {
        //if the animation is already running
        QSequentialAnimationGroup *sag = m_pendingAnimations.value(w);
        if (sag->animationAt(FadeIn)->state() == QAbstractAnimation::Running ||
                sag->animationAt(Show)->state() == QAbstractAnimation::Running ||
                sag->animationAt(FadeOut)->state() == QAbstractAnimation::Running) {
            //if the scrollbar is fully visible, restart the show animation.
            if (sag->animationAt(Show)->state() == QAbstractAnimation::Running) {
                sag->animationAt(Show)->stop();
                sag->animationAt(Show)->start();
            }
        }
    } else {
        const MPositionIndicatorStyle *style =
            static_cast<const MPositionIndicatorStyle *>(QtMaemo6StylePrivate::mStyle(QStyle::State_Active,
                    "MPositionIndicatorStyle"));

        const MWidgetFadeInAnimationStyle *fadeInStyle =
            static_cast<const MWidgetFadeInAnimationStyle *>(QtMaemo6StylePrivate::mStyle(QStyle::State_Active,
                    "MWidgetFadeInAnimationStyle"));

        const MWidgetFadeOutAnimationStyle *fadeOutStyle =
            static_cast<const MWidgetFadeOutAnimationStyle *>(QtMaemo6StylePrivate::mStyle(QStyle::State_Active,
                    "MWidgetFadeOutAnimationStyle"));

        QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup();
        QPropertyAnimation *fadeIn = new QPropertyAnimation(animationGroup);
        fadeIn->setTargetObject(w);
        fadeIn->setPropertyName(property);
        fadeIn->setDuration(fadeInStyle->duration());
        fadeIn->setEasingCurve(fadeInStyle->easingCurve());
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);

        QPropertyAnimation *fadeOut = new QPropertyAnimation(animationGroup);
        fadeOut->setTargetObject(w);
        fadeOut->setPropertyName(property);
        fadeOut->setDuration(fadeOutStyle->duration());
        fadeOut->setEasingCurve(fadeOutStyle->easingCurve());
        fadeOut->setStartValue(1.0);
        fadeOut->setEndValue(0.0);

        animationGroup->insertAnimation(FadeIn, fadeIn);
        animationGroup->insertPause(Show, style->hideTimeout());
        animationGroup->insertAnimation(FadeOut, fadeOut);

        connect(animationGroup->animationAt(FadeIn), SIGNAL(finished()), animationGroup->animationAt(Show), SLOT(start()));
        connect(animationGroup->animationAt(Show), SIGNAL(finished()), animationGroup->animationAt(FadeOut), SLOT(start()));

        //hide must be called before cleanup, because otherwise the widget is deleted before hiding
        connect(animationGroup->animationAt(FadeOut), SIGNAL(finished()), w, SLOT(hide()));
        connect(animationGroup->animationAt(FadeOut), SIGNAL(finished()), this, SLOT(cleanUpTimerMap()));

        connect(fadeIn, SIGNAL(valueChanged(QVariant)), w, SLOT(repaint()));
        connect(fadeOut, SIGNAL(valueChanged(QVariant)), w, SLOT(repaint()));

        w->show();
        m_pendingAnimations.insert(w, animationGroup);

        //do NOT use the animationGroup's start method.
        // if the animation reached the pause animation (show state),
        // the pause animation is restarted every time a scrolling activity
        // occurs. So the pause animation only reaches it's end, if there is
        // no scrolling activity.
        // resetting the animation within the running animation group didn't
        // work!
        fadeIn->start();
    }
}
