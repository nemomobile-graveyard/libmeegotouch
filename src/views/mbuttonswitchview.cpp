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

QSizeF MButtonSwitchViewPrivate::thumbSize() const
{
    Q_Q(const MButtonSwitchView);

    if (q->style()->thumbImage()) {
        QSizeF thumb = q->style()->thumbImage()->pixmap()->size();
        QSizeF view = q->size();

        //scale the thumb to fit inside the view, aspect ratio is kept the same
        if (thumb.height() != view.height()) {
            qreal f = view.height() / thumb.height();
            thumb = thumb * f;
            return QSizeF(thumb.width() - q->style()->thumbMargin() * 2.0, thumb.height() - q->style()->thumbMargin() * 2.0);
        } else
            return thumb;
    }
    return QSizeF();
}

QPointF MButtonSwitchViewPrivate::thumbPos() const
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

QPointF MButtonSwitchViewPrivate::thumbEndPos(bool checked) const
{
    Q_Q(const MButtonSwitchView);
    QSizeF thumb = thumbSize();
    qreal h = (q->size().height() / 2) - (thumb.height() / 2);
    if( checked )
        return QPointF(q->size().width() - thumb.width() - q->style()->thumbMargin(), h);
    else
        return QPointF(q->style()->thumbMargin(), h);
}

void MButtonSwitchViewPrivate::updateHandle()
{
    //invalidate masked slider image and request redraw
    Q_Q(MButtonSwitchView);
    q->update();
}

void MButtonSwitchViewPrivate::drawSwitchSlider(QPainter *painter) const
{
    Q_Q(const MButtonSwitchView);

    qreal oldOpacity = painter->opacity();

    qreal opacity = ((qreal)thumbPos().x() - q->style()->thumbMargin()) / (q->size().width() - thumbSize().width() - q->style()->thumbMargin() * 2.0);
    painter->setOpacity(1.0 - opacity);
    if (q->style()->sliderImage())
        q->style()->sliderImage()->draw(QRectF(QPointF(0,0), q->size()), painter);
    painter->setOpacity(opacity);
    if (q->style()->sliderImageSelected())
        q->style()->sliderImageSelected()->draw(QRectF(QPointF(0,0), q->size()), painter);

    painter->setOpacity(oldOpacity);
}

void MButtonSwitchViewPrivate::drawSwitchThumb(QPainter *painter) const
{
    Q_Q(const MButtonSwitchView);
    if (q->style()->thumbImageShadow())
        q->style()->thumbImageShadow()->draw(thumbPos() + q->style()->thumbShadowOffset(), thumbSize(), painter);
    if (q->style()->thumbImage())
        q->style()->thumbImage()->draw(thumbPos(), thumbSize(), painter);
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
}

void MButtonSwitchView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const MButtonSwitchView);

    d->drawSwitchSlider(painter);
    d->drawSwitchThumb(painter);
}

void MButtonSwitchView::applyStyle()
{
    MButtonView::applyStyle();
}

void MButtonSwitchView::updateData(const QList<const char *>& modifications)
{
    MButtonView::updateData(modifications);
    update();
}

void MButtonSwitchView::setupModel()
{
    MButtonView::setupModel();
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
    update();
}

#include "moc_mbuttonswitchview.cpp"

M_REGISTER_VIEW_NEW(MButtonSwitchView, MButton)
