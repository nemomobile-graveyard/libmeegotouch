/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duibuttonswitchview.h"
#include "duibuttonswitchview_p.h"

#include <QPainter>
#include <QTimeLine>
#include <QDebug>

#include "duiviewcreator.h"
#include "duibutton.h"
#include "duibutton_p.h"
#include "duilabel.h"
#include "duitheme.h"
#include "duidebug.h"
#include "duiscalableimage.h"
//#include "duigles2renderer.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QTimer>


DuiButtonSwitchViewPrivate::DuiButtonSwitchViewPrivate() :
    m_thumbDown(false),
    m_thumbDragged(false),
    m_thumbPos(0, 0)
{
}

DuiButtonSwitchViewPrivate::~DuiButtonSwitchViewPrivate()
{
}

void DuiButtonSwitchViewPrivate::createMaskedSlider()
{
    Q_Q(DuiButtonSwitchView);

    //create images from pixmaps to get access to pixel data

    //TODO this is only SW solution, the proper masking will be implemented with GLES2 shaders

    //scale the mask to match the view size
    if (q->size().isValid()) {
        QPixmap scrollerPm(q->size().width(), q->size().height());
        scrollerPm.fill(QColor(0, 0, 0, 0));
        QPainter p0(&scrollerPm);
        q->style()->sliderMask()->draw(QPoint(0, 0), scrollerPm.size(), &p0);
        p0.end();
        m_maskedSliderImage = scrollerPm.toImage().convertToFormat(QImage::Format_ARGB32);
    }

    //TODO: could be done only when style changes
    m_sliderImage = q->style()->sliderImage()->pixmap()->toImage();
}

void DuiButtonSwitchViewPrivate::updateMaskedSlider()
{
    //TODO this is only SW solution, the proper masking solution will be done with gl shader

    //mask the slider image, result is stored to m_maskedSliderImage
    int width = m_maskedSliderImage.width();
    int height = m_maskedSliderImage.height();
    int offset = (m_sliderImage.width() / 2) - thumbPos().x() - (thumbSize().width() / 2);
    for (int y = 0; y < height; ++y) {
        const uint *scanline = (const uint *) m_sliderImage.scanLine(y) + offset;
        uint *result = (uint *) m_maskedSliderImage.scanLine(y);
        for (int x = 0; x < width; ++x) {
            *result = ((*result) & 0xff000000) | ((*scanline) & 0x00ffffff);
            scanline++;
            result++;
        }
    }
}

QSize DuiButtonSwitchViewPrivate::thumbSize() const
{
    Q_Q(const DuiButtonSwitchView);

    QSize thumb = q->style()->thumbImage()->pixmap()->size();
    QSizeF view = q->size();

    //scale the thumb to fit inside the view, aspect ratio is kept the same
    if (thumb.height() != view.height()) {
        float f = view.height() / thumb.height();
        return thumb * f;
    } else
        return thumb;
}

QPoint DuiButtonSwitchViewPrivate::thumbPos() const
{
    Q_Q(const DuiButtonSwitchView);

    //when thumb is not dragged it is on another end of the view
    if (!m_thumbDragged) {
        QSize thumb = thumbSize();
        int h = (q->size().height() / 2) - (thumb.height() / 2);
        if (q->model()->checked())
            return QPoint(q->size().width() - thumb.width(), h);
        else
            return QPoint(0, h);
    }
    //return dragged thumb position
    else {
        return m_thumbPos;
    }
}

void DuiButtonSwitchViewPrivate::updateImages()
{
    createMaskedSlider();

    //TODO: Is there better solution for this?

    //if the images are not yet valid due to asynchronous resource loading
    //launch singleShot timer until everything has been loaded, this is needed
    //to be able to optimize SW rendering for the masked slider background
    if (m_maskedSliderImage.isNull() || m_sliderImage.isNull() ||
            m_maskedSliderImage.size() == QSize(1, 1) || m_sliderImage.size() == QSize(1, 1)) {
        const int timeout = 500;
        //duiWarning("DuiButtonSwitchViewPrivate") << "updateImages() Images not valid yet, refreshing after" << timeout << "ms";
        Q_Q(DuiButtonSwitchView);
        QTimer::singleShot(timeout, q, SLOT(updateImages()));
    } else
        updateMaskedSlider();
}

DuiButtonSwitchView::DuiButtonSwitchView(DuiButton *controller) :
    DuiButtonView(* new DuiButtonSwitchViewPrivate, controller)
{
}

DuiButtonSwitchView::~DuiButtonSwitchView()
{
}

void DuiButtonSwitchView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    DuiWidgetView::resizeEvent(event);

    Q_D(DuiButtonSwitchView);

    //create and update slider image masking
    d->updateImages();
}

void DuiButtonSwitchView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const DuiButtonSwitchView);

    //((DuiButtonSwitchViewPrivate*)d)->createMaskedSlider();
    //((DuiButtonSwitchViewPrivate*)d)->updateMaskedSlider();

    painter->drawImage(QRect(QPoint(0, 0), size().toSize()), d->m_maskedSliderImage);
    style()->thumbImage()->draw(d->thumbPos(), d->thumbSize(), painter);
}

void DuiButtonSwitchView::applyStyle()
{
    DuiButtonView::applyStyle();

    Q_D(DuiButtonSwitchView);

    //create and update slider image masking
    d->updateImages();
}

void DuiButtonSwitchView::updateData(const QList<const char *>& modifications)
{
    DuiButtonView::updateData(modifications);
    Q_D(DuiButtonSwitchView);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiButtonModel::Checked) {
            d->updateMaskedSlider();
        }
        //else if( member == DuiButtonModel::IconID ) {
        //
        //}
    }
}

void DuiButtonSwitchView::setupModel()
{
    DuiButtonView::setupModel();
    Q_D(DuiButtonSwitchView);
    d->updateMaskedSlider();
}




void DuiButtonSwitchView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //quick escape if the switch is already down
    if (model()->down()) {
        return;
    }

    Q_D(DuiButtonSwitchView);
    QRect thumb(d->thumbPos(), d->thumbSize());

    if (thumb.contains(event->pos().toPoint())) {

        //start drag from ON or OFF position
        d->m_thumbPos = d->thumbPos();


        //if (style()->pressFeedback()) {
        //    style()->pressFeedback()->play();
        //}

        d->mouseOffset = event->pos().x() - d->controller->pos().x() - d->m_thumbPos.x();

        d->m_thumbDown = true;
    }

    //set switch to down mode
    model()->setDown(true);
}



void DuiButtonSwitchView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //DuiButtonView::mouseReleaseEvent(event);
    Q_D(DuiButtonSwitchView);

    Q_UNUSED(event);

    if (!model()->down()) {
        return;
    }

    //check if the thumb has been dragged
    if (d->m_thumbDown && d->m_thumbDragged) {

        d->m_thumbDown = false;
        d->m_thumbDragged = false;
        model()->setDown(false);
        //if (style()->releaseFeedback()) {
        //    style()->releaseFeedback()->play();
        //}

        //QPointF touch = event->scenePos();
        //QRectF rect = d->controller->sceneBoundingRect();
        //rect.adjust(-RELEASE_MISS_DELTA, -RELEASE_MISS_DELTA,
        //            RELEASE_MISS_DELTA, RELEASE_MISS_DELTA);
        //if (rect.contains(touch)) {
        //model()->click();
        if (d->m_thumbPos.x() + (d->thumbSize().width() / 2)  > (size().width() / 2)) {
            model()->setChecked(true);
        } else {
            model()->setChecked(false);
        }
        //}

        d->updateMaskedSlider();
    }
    //user just clicked the switch, act like normal checkable button
    else {
        d->m_thumbDown = false;
        d->m_thumbDragged = false;
        DuiButtonView::mouseReleaseEvent(event);
    }
}

void DuiButtonSwitchView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiButtonSwitchView);

    //drag only if the thumb was pressed down
    if (d->m_thumbDown) {
        QPointF pos = event->pos() - d->controller->pos();
        int x = qRound(pos.x()) - d->mouseOffset;
        if (x < 0)
            x = 0;
        else if (x > (size().width() - d->thumbSize().width()))
            x = size().width() - d->thumbSize().width();

        d->m_thumbPos.setX(x);
        d->m_thumbDragged = true;
        d->updateMaskedSlider();

        update();
    }
}

void DuiButtonSwitchView::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);
    model()->setDown(false);
    update();
}

#include "moc_duibuttonswitchview.cpp"

DUI_REGISTER_VIEW_NEW(DuiButtonSwitchView, DuiButton)
