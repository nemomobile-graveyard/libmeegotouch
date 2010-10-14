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

#include "mbuttonswitchview.h"
#include "mbuttonswitchview_p.h"

#include "mbutton.h"
#include "mbutton_p.h"
#include "mscalableimage.h"
#include "mviewconstants.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QVariantAnimation>

//! \internal
class HandleAnimation : public QVariantAnimation
{
    public:
        HandleAnimation(MButtonSwitchViewPrivate* viewPrivate, QObject * parent = 0)
            : QVariantAnimation(parent), m_viewPrivate(viewPrivate)
            {
            }

    protected:

        virtual void updateCurrentValue(const QVariant& value)
        {
            m_viewPrivate->m_thumbPos.setX(value.toPoint().x());
            m_viewPrivate->updateHandle();
        }

    private:
        MButtonSwitchViewPrivate* m_viewPrivate;
};
//! \internal_end

MButtonSwitchViewPrivate::MButtonSwitchViewPrivate() :
    mouseOffset(0),
    m_thumbDown(false),
    m_thumbDragged(false),
    m_feedbackOnPlayed(false),
    m_thumbPos(),
    m_thumbPosValid(false),
    m_handleAnimation(new HandleAnimation(this)),
    m_animationSpeed(500)
{
}

MButtonSwitchViewPrivate::~MButtonSwitchViewPrivate()
{
    delete m_handleAnimation;
}

QSize MButtonSwitchViewPrivate::thumbSize() const
{
    Q_Q(const MButtonSwitchView);

    QSize thumb = q->style()->thumbImage()->pixmap()->size();
    QSizeF view = q->size();

    //scale the thumb to fit inside the view, aspect ratio is kept the same
    if (thumb.height() != view.height()) {
        float f = view.height() / thumb.height();
        thumb = thumb * f;
        return QSize(thumb.width() - q->style()->thumbMargin() * 2, thumb.height() - q->style()->thumbMargin() * 2);
    } else
        return thumb;
}

QPoint MButtonSwitchViewPrivate::thumbPos() const
{
    Q_Q(const MButtonSwitchView);

    //when thumb is not dragged it is on another end of the view
    if (!m_thumbDown && m_handleAnimation->state() == QAbstractAnimation::Stopped) {
        return thumbEndPos(q->model()->checked());
    }
    //return dragged thumb position
    else {
        return m_thumbPos;
    }
}

QPoint MButtonSwitchViewPrivate::thumbEndPos(bool checked) const
{
    Q_Q(const MButtonSwitchView);
    QSize thumb = thumbSize();
    int h = (q->size().height() / 2) - (thumb.height() / 2);
    if( checked )
        return QPoint(q->size().width() - thumb.width() - q->style()->thumbMargin(), h);
    else
        return QPoint(q->style()->thumbMargin(), h);
}


const QPixmap& MButtonSwitchViewPrivate::maskedSliderImage() const
{
    //create new masked slider image if it has been invalidated
    if (m_maskedSliderPm.isNull() && !MTheme::hasPendingRequests()) {

        //create the new masked slider image only if the source images are valid
        Q_Q(const MButtonSwitchView);
        if (q->style()->sliderImage() &&
            q->style()->sliderImageSelected() &&
            !q->style()->sliderImage()->pixmap()->isNull() &&
            !q->style()->sliderImageSelected()->pixmap()->isNull() &&
            !q->style()->sliderMask()->pixmap()->isNull() &&
            q->size().toSize().width() != thumbSize().width()) {

            //create image and make it totally transparent
            m_maskedSliderPm = QPixmap(q->size().toSize());
            m_maskedSliderPm.fill(Qt::transparent);

            //create the masked slider image using MScalableImage::draw() overload
            QPainter p;

            // blend backgrounds
            QPixmap backgroundsBlend = QPixmap(q->size().toSize());
            backgroundsBlend.fill(Qt::transparent);
            p.begin(&backgroundsBlend);
            qreal opacity = ((qreal)thumbPos().x() - q->style()->thumbMargin()) / (q->size().width() - thumbSize().width() - q->style()->thumbMargin() * 2.0);
            p.setOpacity(1.0 - opacity);
            p.drawPixmap(QRect(QPoint(), q->size().toSize()), *q->style()->sliderImage()->pixmap());
            p.setOpacity(opacity);
            p.drawPixmap(QRect(QPoint(), q->size().toSize()), *q->style()->sliderImageSelected()->pixmap());
            p.end();

            p.begin(&m_maskedSliderPm);
            q->style()->sliderMask()->draw(QRect(QPoint(0,0), q->size().toSize()), QPoint(0,0), &backgroundsBlend, &p);
            p.end();
        }
    }

    return m_maskedSliderPm;
}

void MButtonSwitchViewPrivate::updateHandle()
{
    //invalidate masked slider image and request redraw
    Q_Q(MButtonSwitchView);
    m_maskedSliderPm = QPixmap();
    q->update();
}


MButtonSwitchView::MButtonSwitchView(MButton *controller) :
    MButtonView(* new MButtonSwitchViewPrivate, controller)
{
}

MButtonSwitchView::~MButtonSwitchView()
{
}

void MButtonSwitchView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MWidgetView::resizeEvent(event);

    //invalidate masked slider image
    Q_D(MButtonSwitchView);
    d->m_maskedSliderPm = QPixmap();
}

void MButtonSwitchView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const MButtonSwitchView);

    painter->drawPixmap(QRect(QPoint(0, 0), size().toSize()), d->maskedSliderImage());
    style()->thumbImage()->draw(d->thumbPos(), d->thumbSize(), painter);
}

void MButtonSwitchView::applyStyle()
{
    MButtonView::applyStyle();

    //invalidate masked slider image
    Q_D(MButtonSwitchView);
    d->m_maskedSliderPm = QPixmap();
}

void MButtonSwitchView::updateData(const QList<const char *>& modifications)
{
    MButtonView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == MButtonModel::Checked) {
            //invalidate masked slider image
            Q_D(MButtonSwitchView);
            d->m_maskedSliderPm = QPixmap();
        }
    }
}

void MButtonSwitchView::setupModel()
{
    MButtonView::setupModel();

    //invalidate masked slider image
    Q_D(MButtonSwitchView);

    //d->m_thumbPos = d->thumbEndPos(model()->checked());
    d->m_maskedSliderPm = QPixmap();
    update();
}

void MButtonSwitchView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //quick escape if the switch is already down
    if (model()->down()) {
        return;
    }

    Q_D(MButtonSwitchView);

    //stop ongoing animation if any
    d->m_handleAnimation->stop();

    //honor MWidgetView's style and play press feedback
    style()->pressFeedback().play();

    //play switch feedback effect
    if (model()->checked() == false) {
        style()->pressOnFeedback().play();
        d->m_feedbackOnPlayed = false;
    } else {
        style()->pressOffFeedback().play();
        d->m_feedbackOnPlayed = true;
    }

    //init the the thumb position to valid value when we first time interract
    //with the button, this needs to be done here because we need the size of the
    //thumb image in our calculations and due to asynchronous pixmap loading it
    //may not be ready before this
    if( !d->m_thumbPosValid ) {
        d->m_thumbPosValid = true;
        d->m_thumbPos = d->thumbEndPos(model()->checked());
    }

    //calculate offset from edge of the thumb to the drag point
    d->mouseOffset = event->pos().x() - d->controller->pos().x() - d->m_thumbPos.x();

    //set switch to down mode
    d->m_thumbDown = true;
    model()->setDown(true);
}

void MButtonSwitchView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MButtonSwitchView);

    Q_UNUSED(event);

    if (!model()->down()) {
        return;
    }
    model()->setDown(false);

    //honor MWidgetView's style and play release feedback
    style()->releaseFeedback().play();

    //check if the thumb has been dragged
    if (d->m_thumbDown && d->m_thumbDragged) {
        //stop dragging
        d->m_thumbDown = false;
        d->m_thumbDragged = false;
        model()->setDown(false);

        //set the state depending which side the thumb currently is
        if (d->m_thumbPos.x() + (d->thumbSize().width() / 2)  > (size().width() / 2)) {
            model()->setChecked(true);
        } else {
            model()->setChecked(false);
        }

        //invalidate masked slider image
        d->m_maskedSliderPm = QPixmap();
    }
    //user just clicked the switch, act like normal checkable button
    else {
        d->m_thumbDown = false;
        d->m_thumbDragged = false;

        QPointF touch = event->scenePos();
        QRectF rect = d->controller->sceneBoundingRect();
        rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                    M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
        if (rect.contains(touch))
            model()->click();

        if (model()->checked() == true) {
            style()->releaseOnFeedback().play();
        } else {
            style()->releaseOffFeedback().play();
        }
    }

    //start animating the thumb from current position to proper end position
    int delta = d->m_thumbPos.x() - d->thumbEndPos(model()->checked()).x();
    d->m_handleAnimation->setStartValue(d->m_thumbPos);
    d->m_handleAnimation->setEndValue(d->thumbEndPos(model()->checked()));
    d->m_handleAnimation->setDuration(abs(delta) / (d->m_animationSpeed/1000.0));
    d->m_handleAnimation->start();
}

void MButtonSwitchView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MButtonSwitchView);

    //drag only if the thumb was pressed down
    if (d->m_thumbDown) {
        //calculate new x for the thumb
        QPointF pos = event->pos() - d->controller->pos();
        int x = qRound(pos.x()) - d->mouseOffset;

        //keep the thumb totally inside the view
        if (x < style()->thumbMargin())
            x = style()->thumbMargin();
        else if (x > (size().width() - d->thumbSize().width() - style()->thumbMargin()))
            x = size().width() - d->thumbSize().width() - style()->thumbMargin();
        d->m_thumbPos.setX(x);
        d->m_thumbDragged = true;

        //invalidate masked slider image
        d->m_maskedSliderPm = QPixmap();

        //play switch feedback effect if center position is crossed
        if( d->m_thumbPos.x() + (d->thumbSize().width() / 2)  > (size().width() / 2) ) {
            if (d->m_feedbackOnPlayed == false) {
                style()->releaseOnFeedback().play();
                d->m_feedbackOnPlayed = true;
            }
        } else {
            if (d->m_feedbackOnPlayed == true) {
                style()->releaseOffFeedback().play();
                d->m_feedbackOnPlayed = false;
            }
        }

        update();
    }
}

void MButtonSwitchView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MButtonSwitchView);

    style()->cancelFeedback().play();

    model()->setDown(false);
    d->m_thumbDown = false;
    d->m_thumbDragged = false;

    d->m_maskedSliderPm = QPixmap();
    update();
}

#include "moc_mbuttonswitchview.cpp"

M_REGISTER_VIEW_NEW(MButtonSwitchView, MButton)
