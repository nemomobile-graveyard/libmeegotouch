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

#include "msliderview.h"
#include "msliderview_p.h"

#include <math.h>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QSizeF>
#include <QApplication>
#include <QGraphicsAnchorLayout>
#include <QPropertyAnimation>
#include <limits>

#include "mdebug.h"

#include "mtheme.h"
#include "mscalableimage.h"
#include "mwidgetview.h"
#include "mslider.h"
#include "mseekbar.h"
#include "mslider_p.h"
#include "mviewcreator.h"
#include "mlabel.h"
#include "mimagewidget.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"

MSliderHandle::MSliderHandle(QGraphicsItem *parent) :
    MWidget(parent),
    orientation(Qt::Horizontal),
    handlePixmap(0),
    handlePressedPixmap(0),
    handleVerticalPixmap(0),
    handleVerticalPressedPixmap(0),
    sliderState(MSliderModel::Released)
{
}

MSliderHandle::~MSliderHandle()
{
}

void MSliderHandle::setOrientation(Qt::Orientation orientation)
{
    this->orientation = orientation;
    updateGeometry();
}

void MSliderHandle::setPixmaps(const QPixmap *handle,
                                 const QPixmap *handlePressed,
                                 const QPixmap *handleVertical,
                                 const QPixmap *handleVerticalPressed)
{
    handlePixmap = handle;
    handlePressedPixmap = handlePressed;
    handleVerticalPixmap = handleVertical;
    handleVerticalPressedPixmap = handleVerticalPressed;

    updateGeometry();
}

void MSliderHandle::setSliderState(MSliderModel::SliderState state)
{
    sliderState = state;
    updateGeometry();
}

void MSliderHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRect handleRect = rect().toRect();
    if (sliderState == MSliderModel::Pressed) {
        if (orientation == Qt::Horizontal)
            painter->drawPixmap((handleRect.width() - handlePressedPixmap->width()) / 2,
                                (handleRect.height() - handlePressedPixmap->height()) / 2,
                                *handlePressedPixmap);
        if (orientation == Qt::Vertical)
            painter->drawPixmap((handleRect.width() - handleVerticalPressedPixmap->width()) / 2,
                                (handleRect.height() - handleVerticalPressedPixmap->height()) / 2,
                                *handleVerticalPressedPixmap);
    }
    if (sliderState == MSliderModel::Released) {
        if (orientation == Qt::Horizontal)
            painter->drawPixmap((handleRect.width() - handlePixmap->width()) / 2,
                                (handleRect.height() - handlePixmap->height()) / 2,
                                *handlePixmap);
        if (orientation == Qt::Vertical)
            painter->drawPixmap((handleRect.width() - handleVerticalPixmap->width()) / 2,
                                (handleRect.height() - handleVerticalPixmap->height()) / 2,
                                *handleVerticalPixmap);

    }
}

QSizeF MSliderHandle::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    int width = 0;
    int height = 0;

    if (orientation == Qt::Horizontal) {
        if (sliderState == MSliderModel::Released) {
            if (handlePixmap) {
                width = qMax(width, handlePixmap->width());
                height = qMax(height, handlePixmap->height());
            }
        }

        if (sliderState == MSliderModel::Pressed) {
            if (handlePressedPixmap) {
                width = qMax(width, handlePressedPixmap->width());
                height = qMax(height, handlePressedPixmap->height());
            }
        }
    }

    if (orientation == Qt::Vertical) {
        if (sliderState == MSliderModel::Released) {
            if (handleVerticalPixmap) {
                width = qMax(width, handleVerticalPixmap->width());
                height = qMax(height, handleVerticalPixmap->height());
            }
        }

        if (sliderState == MSliderModel::Pressed) {
            if (handleVerticalPressedPixmap) {
                width = qMax(width, handleVerticalPressedPixmap->width());
                height = qMax(height, handleVerticalPressedPixmap->height());
            }
        }
    }

    return QSizeF(width, height);
}

MSliderIndicator::MSliderIndicator(bool isMinMax, QGraphicsItem *parent) :
    MWidget(parent),
    label(0),
    image(0),
    layout(0)
{
    layout = new QGraphicsAnchorLayout;
    setLayout(layout);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    label = new MLabel;
    if (isMinMax)
        label->setObjectName("MSliderMinMaxLabel");
    else
        label->setObjectName("MSliderHandleLabel");

    image = new MImageWidget;
    image->setObjectName("MSliderImage");

    layout->addAnchor(layout, Qt::AnchorVerticalCenter, label, Qt::AnchorVerticalCenter);
    layout->addAnchor(layout, Qt::AnchorVerticalCenter, image, Qt::AnchorVerticalCenter);

    label->resize(0, 0);
    image->resize(0, 0);
}

MSliderIndicator::~MSliderIndicator()
{
}

void MSliderIndicator::setText(const QString &text)
{
    label->setVisible(!text.isEmpty());

    label->setText(text);
    while (layout->count() > 0)
        layout->removeAt(0);

    layout->addAnchor(layout, Qt::AnchorVerticalCenter, label, Qt::AnchorVerticalCenter);
    layout->addAnchor(layout, Qt::AnchorVerticalCenter, image, Qt::AnchorVerticalCenter);

    if (text.isEmpty()) {
        label->setMinimumSize(0, 0);
        label->setPreferredSize(0, 0);
        label->setMaximumSize(0, 0);
    } else {
        label->setMinimumSize(label->sizeHint(Qt::PreferredSize));
        label->setPreferredSize(label->sizeHint(Qt::PreferredSize));
        label->setMaximumSize(label->sizeHint(Qt::PreferredSize));
    }

    label->resize(label->sizeHint(Qt::PreferredSize));

    updateGeometry();
}

void MSliderIndicator::setImage(const QString &id)
{
    image->setVisible(!id.isEmpty());

    imageName = id;

    QPixmap *pixmap = 0;
    if (!id.isEmpty()) {
        pixmap = MTheme::pixmapCopy(id);
        image->setPixmap(*pixmap);
        delete pixmap;
    } else {
        pixmap = new QPixmap();
        image->setPixmap(*pixmap);
        delete pixmap;
    }

    while (layout->count() > 0)
        layout->removeAt(0);

    layout->addAnchor(layout, Qt::AnchorVerticalCenter, label, Qt::AnchorVerticalCenter);
    layout->addAnchor(layout, Qt::AnchorVerticalCenter, image, Qt::AnchorVerticalCenter);

    if (id.isEmpty()) {
        image->setMinimumSize(0, 0);
        image->setPreferredSize(0, 0);
        image->setMaximumSize(0, 0);
    } else {
        image->setMinimumSize(image->sizeHint(Qt::PreferredSize));
        image->setPreferredSize(image->sizeHint(Qt::PreferredSize));
        image->setMaximumSize(image->sizeHint(Qt::PreferredSize));
    }

    image->resize(image->sizeHint(Qt::PreferredSize));

    updateGeometry();
}

QSizeF MSliderIndicator::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    qreal width = 0;
    qreal height = 0;

    if (image && !imageName.isEmpty()) {
        width = qMax(width, image->sizeHint(Qt::PreferredSize).width());
        height = qMax(height, image->sizeHint(Qt::PreferredSize).height());
    }

    if (label && !label->text().isEmpty()) {
        width = qMax(width, label->sizeHint(Qt::PreferredSize).width());
        height = qMax(height, label->sizeHint(Qt::PreferredSize).height());
    }

    return QSizeF(width, height);
}

MSliderGroove::MSliderGroove(QGraphicsItem *parent) :
    MWidget(parent),
    controller(0),
    orientation(Qt::Horizontal),
    backgroundBaseImage(0),
    backgroundElapsedImage(0),
    backgroundReceivedImage(0),
    backgroundVerticalBaseImage(0),
    backgroundVerticalElapsedImage(0),
    backgroundVerticalReceivedImage(0),
    grooveThickness(0),
    minimum(0),
    maximum(0),
    value(0),
    loadedContentMinimum(0),
    loadedContentMaximum(0),
    showSeekBar(false),
    sliderHandle(0),
    sliderHandleIndicator(0),
    preferredLength(0),
    minimumLength(0),
    maximumLength(0)
{
    sliderHandle = new MSliderHandle(this);
    sliderHandle->setObjectName("MSliderHandle");
    
    sliderHandleIndicator = new MSliderIndicator(false, this);
    sliderHandleIndicator->setObjectName("MSliderHandleIndicator");
    
    sliderHandleIndicator->setVisible(false);
}

MSliderGroove::~MSliderGroove()
{
    ensureSafeClosing();
}

void MSliderGroove::init(MSlider *controller)
{
    this->controller = controller;
}

void MSliderGroove::setOrientation(Qt::Orientation orientation)
{
    this->orientation = orientation;
    sliderHandle->setOrientation(orientation);

    updateGeometry();
}

void MSliderGroove::setHandlePixmaps(const QPixmap *handle,
                                       const QPixmap *handlePressed,
                                       const QPixmap *handleVertical,
                                       const QPixmap *handleVerticalPressed)
{
    sliderHandle->setPixmaps(handle,
                             handlePressed,
                             handleVertical,
                             handleVerticalPressed);

    updateGeometry();
}

void MSliderGroove::setImages(const MScalableImage *background,
                                const MScalableImage *backgroundElapsed,
                                const MScalableImage *backgroundReceived,
                                const MScalableImage *backgroundVertical,
                                const MScalableImage *backgroundVerticalElapsed,
                                const MScalableImage *backgroundVerticalReceived)
{
    backgroundBaseImage = background;
    backgroundElapsedImage = backgroundElapsed;
    backgroundReceivedImage = backgroundReceived;
    backgroundVerticalBaseImage = backgroundVertical;
    backgroundVerticalElapsedImage = backgroundVerticalElapsed;
    backgroundVerticalReceivedImage = backgroundVerticalReceived;

    updateGeometry();
}

void MSliderGroove::setGrooveThickness(qreal thickness)
{
    grooveThickness = thickness;

    updateGeometry();
}

void MSliderGroove::setGrooveLength(qreal prefLength, qreal minLength, qreal maxLength)
{
    preferredLength = prefLength;
    minimumLength = minLength;
    maximumLength = maxLength;

    updateGeometry();
}

void MSliderGroove::setSliderValues(int min, int max, int val)
{
    if (min < max) {
        minimum = min;
        maximum = max;
    } else {
        minimum = max;
        maximum = min;
    }

    value = val;
    value = qBound(minimum, value, maximum);

    QPointF handlePos;

    if (orientation == Qt::Horizontal)
        handlePos.setX(valueToScreenCoordinate(value));
    else
        handlePos.setY(valueToScreenCoordinate(value));
    updateHandlePos(handlePos);

    update();
}

void MSliderGroove::setSeekBarValues(bool show, int loadedContentMin, int loadedContentMax)
{
    showSeekBar = show;

    if (loadedContentMin < loadedContentMax) {
        loadedContentMinimum = loadedContentMin;
        loadedContentMaximum = loadedContentMax;
    } else {
        loadedContentMinimum = loadedContentMax;
        loadedContentMaximum = loadedContentMin;
    }

    loadedContentMinimum = qBound(minimum, loadedContentMinimum, maximum);
    loadedContentMaximum = qBound(minimum, loadedContentMaximum, maximum);

    update();
}

void MSliderGroove::setSliderState(MSliderModel::SliderState state)
{
    sliderHandle->setSliderState(state);
    updateHandleIndicatorPos();
}

void MSliderGroove::setIndicatorText(const QString &text)
{
    sliderHandleIndicator->setText(text);
    updateHandleIndicatorPos();
}

void MSliderGroove::setIndicatorImage(const QString &id)
{
    sliderHandleIndicator->setImage(id);
    updateHandleIndicatorPos();
}

//converts one of coordinates of point to slider value
int MSliderGroove::screenPointToValue(const QPointF &point) const
{
    bool reverse = qApp->isRightToLeft();
    QPointF handlePoint = point - pos();

    qreal coordinate = 0;

    if (orientation == Qt::Horizontal)
        coordinate = handlePoint.x();
    if (orientation == Qt::Vertical)
        coordinate = handlePoint.y();

    int range = maximum - minimum;
    int offset = 0;

    if (minimum != maximum) {
        QRectF valueRangeRect = rect();

        if (orientation == Qt::Horizontal) {
            qreal hAdjustment = sliderHandle->rect().width() / 2;

            valueRangeRect.adjust(hAdjustment, 0, -hAdjustment, 0);
            coordinate = qBound(valueRangeRect.left(), coordinate, valueRangeRect.right());

            if (!reverse)
                offset = qRound(((coordinate - valueRangeRect.left()) * range) / valueRangeRect.width());
            else
                offset = qRound(((valueRangeRect.right() - coordinate) * range) / valueRangeRect.width());
        }

        if (orientation == Qt::Vertical) {
            qreal vAdjustment = sliderHandle->rect().height() / 2;

            valueRangeRect.adjust(0, vAdjustment, 0, -vAdjustment);
            coordinate = qBound(valueRangeRect.top(), coordinate, valueRangeRect.bottom());

            offset = qRound(((valueRangeRect.bottom() - coordinate) * range) / valueRangeRect.height());
        }
    }

    return minimum + offset;
}

//determines clickable area which is basically the
//exact area of slider groove
QRectF MSliderGroove::clickableArea() const
{
    QRectF grooveRect = rect();
    if (!grooveRect.isValid())
        return grooveRect;

    if (orientation == Qt::Horizontal) {
        qreal hAdjustment = (sliderHandle->rect().width() - grooveThickness) / 2;
        if (hAdjustment < 0)
            hAdjustment = 0;

        grooveRect.adjust(hAdjustment, 0, -hAdjustment, 0);
    }

    if (orientation == Qt::Vertical) {
        qreal vAdjustment = (sliderHandle->rect().height() - grooveThickness) / 2;
        if (vAdjustment < 0)
            vAdjustment = 0;

        grooveRect.adjust(0, vAdjustment, 0, -vAdjustment);
    }

    return grooveRect;
}

//area occupied by slider handle (user can)
//click both of them
QRectF MSliderGroove::clickableHandleArea() const
{
    QRectF handleRect = sliderHandle->rect();

    handleRect.translate(sliderHandle->pos());
    return handleRect;
}

QRectF MSliderGroove::boundingRect() const
{
    return sliderBoundingRect;
}

void MSliderGroove::raiseHandleIndicator()
{
    QGraphicsItem *newParent = topLevelItem();
    if (newParent) {
        sliderHandleIndicator->setParentItem(newParent);

        //by raising handle indicator will be shown
        sliderHandleIndicator->setVisible(true);
    }

    updateHandleIndicatorPos();
}

void MSliderGroove::lowerHandleIndicator()
{
    //by lowering  handle indicator will be also hidden
    sliderHandleIndicator->setVisible(false);

    sliderHandleIndicator->setParentItem(this);

    updateHandleIndicatorPos();
}

void MSliderGroove::ensureSafeClosing()
{
    sliderHandleIndicator->setParentItem(this);
}

void MSliderGroove::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    bool reverse = qApp->isRightToLeft();

    QRectF grooveRect = rect();
    if (!grooveRect.isValid())
        return;

    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;

    int horizontalMargins = 0;
    int verticalMargins = 0;

    if (orientation == Qt::Horizontal) {
        if (backgroundBaseImage) {
            backgroundBaseImage->borders(&left, &right, &top, &bottom);
            horizontalMargins = left + right;
        }

        qreal hAdjustment = (sliderHandle->rect().width() - grooveThickness) / 2;
        if (hAdjustment < 0)
            hAdjustment = 0;

        qreal vAdjustment = (grooveRect.height() - grooveThickness) / 2;

        grooveRect.adjust(hAdjustment, vAdjustment, -hAdjustment, -vAdjustment);

        if (backgroundBaseImage) {
            if (grooveRect.width() >= qreal(horizontalMargins))
                backgroundBaseImage->draw(grooveRect.toRect(), painter);
        }

        if (showSeekBar) {
            if (backgroundReceivedImage) {
                QRectF receivedRect = grooveRect;

                if (!reverse) {
                    receivedRect.setLeft(valueToScreenCoordinate(loadedContentMinimum));
                    receivedRect.setRight(valueToScreenCoordinate(loadedContentMaximum));

                    if (loadedContentMinimum == minimum)
                        receivedRect.setLeft(grooveRect.left());
                    if (loadedContentMaximum == maximum)
                        receivedRect.setRight(grooveRect.right());
                } else {
                    receivedRect.setLeft(valueToScreenCoordinate(loadedContentMaximum));
                    receivedRect.setRight(valueToScreenCoordinate(loadedContentMinimum));

                    if (loadedContentMinimum == minimum)
                        receivedRect.setRight(grooveRect.right());
                    if (loadedContentMaximum == maximum)
                        receivedRect.setLeft(grooveRect.left());
                }

                if (receivedRect.width() >= qreal(horizontalMargins))
                    backgroundReceivedImage->draw(receivedRect.toRect(), painter);
            }
        }

        if (backgroundElapsedImage) {
            QRectF elapsedRect = grooveRect;

            if (!reverse) {
                if (value < maximum)
                    elapsedRect.setRight(valueToScreenCoordinate(value));
            }
            else {
                if (value < maximum)
                    elapsedRect.setLeft(valueToScreenCoordinate(value));
            }

            if (elapsedRect.width() >= qreal(horizontalMargins))
                backgroundElapsedImage->draw(elapsedRect.toRect(), painter);
        }
    }

    if (orientation == Qt::Vertical) {
        if (backgroundVerticalBaseImage) {
            backgroundVerticalBaseImage->borders(&left, &right, &top, &bottom);
            verticalMargins = top + bottom;
        }

        qreal vAdjustment = (sliderHandle->rect().width() - grooveThickness) / 2;
        if (vAdjustment < 0)
            vAdjustment = 0;

        qreal hAdjustment = (grooveRect.width() - grooveThickness) / 2;

        grooveRect.adjust(hAdjustment, vAdjustment, -hAdjustment, -vAdjustment);

        if (backgroundVerticalBaseImage) {
            if (grooveRect.width() >= qreal(verticalMargins))
                backgroundVerticalBaseImage->draw(grooveRect.toRect(), painter);
        }

        if (showSeekBar) {
            if (backgroundVerticalReceivedImage) {
                QRectF receivedRect = grooveRect;

                receivedRect.setTop(valueToScreenCoordinate(loadedContentMaximum));
                receivedRect.setBottom(valueToScreenCoordinate(loadedContentMinimum));

                if (loadedContentMinimum == minimum)
                    receivedRect.setBottom(grooveRect.bottom());
                if (loadedContentMaximum == maximum)
                    receivedRect.setTop(grooveRect.top());

                if (receivedRect.height() >= qreal(verticalMargins))
                    backgroundVerticalReceivedImage->draw(receivedRect.toRect(), painter);
            }
        }

        if (backgroundVerticalElapsedImage) {
            QRectF elapsedRect = grooveRect;

            if (value < maximum) {
                elapsedRect.setTop(valueToScreenCoordinate(value));
            }

            if (elapsedRect.height() >= qreal(verticalMargins))
                backgroundVerticalElapsedImage->draw(elapsedRect.toRect(), painter);
        }
    }
}

//repositions slider handle (and slider handle indicator)
void MSliderGroove::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);

    QPointF handlePos;

    if (orientation == Qt::Horizontal)
        handlePos.setX(valueToScreenCoordinate(value));
    if (orientation == Qt::Vertical)
        handlePos.setY(valueToScreenCoordinate(value));

    updateHandlePos(handlePos);
}

QSizeF MSliderGroove::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);

    switch (which) {
    case Qt::MinimumSize: {
        if (orientation == Qt::Horizontal)
            return QSizeF(minimumLength, sliderHandle->sizeHint(Qt::PreferredSize).height());
        if (orientation == Qt::Vertical)
            return QSizeF(sliderHandle->sizeHint(Qt::PreferredSize).width(), minimumLength);

        break;
    }
    case Qt::PreferredSize: {
        if (orientation == Qt::Horizontal)
            return QSizeF(preferredLength, sliderHandle->sizeHint(Qt::PreferredSize).height());
        if (orientation == Qt::Vertical)
            return QSizeF(sliderHandle->sizeHint(Qt::PreferredSize).width(), preferredLength);

        break;
    }
    case Qt::MaximumSize: {
        if (orientation == Qt::Horizontal)
            return QSizeF(maximumLength, sliderHandle->sizeHint(Qt::PreferredSize).height());
        if (orientation == Qt::Vertical)
            return QSizeF(sliderHandle->sizeHint(Qt::PreferredSize).width(), maximumLength);

        break;
    }
    default:
        mWarning("MSliderView") << "MSliderView::sizeHint() don't know how to handle the value of 'which' ";
        break;
    }

    return QSizeF(0, 0);
}

//called when slider groove is repositioned or
//resized (overidden to reposition slider handle
//(and slider handle indicator))
void MSliderGroove::setGeometry(const QRectF &rect)
{
    MWidget::setGeometry(rect);

    setSliderValues(minimum, maximum, value);
    setSeekBarValues(showSeekBar, loadedContentMinimum, loadedContentMaximum);
}

//sets middle point so that the whole rect occupied
//by handle remains inside rect occupied by groove
void MSliderGroove::updateHandlePos(const QPointF &position)
{
    if (orientation == Qt::Horizontal) {
        qreal x = position.x();

        x = qBound(sliderHandle->rect().width() / 2, x, rect().width() - (sliderHandle->rect().width() / 2));

        sliderHandle->setPos(x - (sliderHandle->rect().width() / 2), (rect().height() - sliderHandle->rect().height()) / 2);
    }

    if (orientation == Qt::Vertical) {
        qreal y = position.y();

        y = qBound(sliderHandle->rect().height() / 2, y, rect().height() - (sliderHandle->rect().height() / 2));

        sliderHandle->setPos((rect().width() - sliderHandle->rect().width()) / 2, y - (sliderHandle->rect().height() / 2));
    }
    updateHandleIndicatorPos();
}

//updates slider handle indicator accordingly to
//slider handle position, orientation and scene
//direction
void MSliderGroove::updateHandleIndicatorPos()
{
    if (!controller)
        return;

    //slider handle indicator position will not be updated
    //if it is not visible
    if (!sliderHandleIndicator->isVisible())
        return;

    bool reverse = qApp->isRightToLeft();

    QPointF handleIndicatorPos = sliderHandle->pos();

    if (orientation == Qt::Horizontal) {
        handleIndicatorPos.setX(handleIndicatorPos.x() +
                                (sliderHandle->rect().width() / 2) - (sliderHandleIndicator->rect().width() / 2));
        handleIndicatorPos.setY(handleIndicatorPos.y() - sliderHandleIndicator->rect().height());
    }
    if (orientation == Qt::Vertical) {
        if (!reverse)
            handleIndicatorPos.setX(handleIndicatorPos.x() + sliderHandle->rect().width());
        else
            handleIndicatorPos.setX(handleIndicatorPos.x() - sliderHandleIndicator->rect().width());

        handleIndicatorPos.setY(handleIndicatorPos.y() +
                                (sliderHandle->rect().height() / 2) - (sliderHandleIndicator->rect().height() / 2));
    }

    QPointF groovePos = pos();
    sliderBoundingRect = controller->rect();
    sliderBoundingRect.translate(-groovePos);

    if (orientation == Qt::Horizontal) {
        if (handleIndicatorPos.x() < sliderBoundingRect.left())
            handleIndicatorPos.setX(sliderBoundingRect.left());
        if (handleIndicatorPos.x() + sliderHandleIndicator->rect().width() > sliderBoundingRect.right() &&
                handleIndicatorPos.x() > sliderBoundingRect.left())
            handleIndicatorPos.setX(sliderBoundingRect.right() - sliderHandleIndicator->rect().width());
    }
    if (orientation == Qt::Vertical) {
        if (handleIndicatorPos.y() < sliderHandleIndicator->rect().top())
            handleIndicatorPos.setY(sliderBoundingRect.top());
        if (handleIndicatorPos.y() + sliderHandleIndicator->rect().height() > sliderBoundingRect.bottom() &&
                handleIndicatorPos.y() > sliderBoundingRect.top())
            handleIndicatorPos.setY(sliderBoundingRect.bottom() - sliderHandleIndicator->rect().height());
    }

    QGraphicsItem *sliderHandleIndicatorParent = sliderHandleIndicator->parentItem();
    QPointF grooveRelativePos;
    if (sliderHandleIndicatorParent)
        grooveRelativePos = mapFromScene(scenePos()) - mapFromScene(sliderHandleIndicatorParent->scenePos());

    sliderHandleIndicator->setPos(grooveRelativePos + handleIndicatorPos);

    //recalculates bounding rect according to area occupied by handle indicator
    //because that area still has to be redrawn whenever slider is redrawn
    QRectF sliderHandleIndicatorRect = sliderHandleIndicator->rect();
    sliderHandleIndicatorRect.translate(sliderHandleIndicator->pos());

    sliderBoundingRect.setLeft(qMin(sliderBoundingRect.left(), sliderHandleIndicatorRect.left()));
    sliderBoundingRect.setTop(qMin(sliderBoundingRect.top(), sliderHandleIndicatorRect.top()));
    sliderBoundingRect.setRight(qMax(sliderBoundingRect.right(), sliderHandleIndicatorRect.right()));
    sliderBoundingRect.setBottom(qMax(sliderBoundingRect.bottom(), sliderHandleIndicatorRect.bottom()));

    prepareGeometryChange();
}

//converts value to scene coordinate (x coordinate for
//horizontal sliders and y coordinate for vertical sliders)
qreal MSliderGroove::valueToScreenCoordinate(int value) const
{
    bool reverse = qApp->isRightToLeft();
    qreal beginning = 0;

    int range = maximum - minimum;
    qreal offset = 0;
    QRectF valueRangeRect = rect();

    if (minimum != maximum) {
        if (orientation == Qt::Horizontal) {
            qreal hAdjustment = sliderHandle->rect().width() / 2;

            valueRangeRect.adjust(hAdjustment, 0, -hAdjustment, 0);
            beginning = valueRangeRect.left();

            if (!reverse)
                offset = (value - minimum) * valueRangeRect.width() / range;
            else
                offset = (maximum - value) * valueRangeRect.width() / range;
        }

        if (orientation == Qt::Vertical) {
            qreal vAdjustment = sliderHandle->rect().height() / 2;

            valueRangeRect.adjust(0, vAdjustment, 0, -vAdjustment);
            beginning = valueRangeRect.top();

            offset = (maximum - value) * valueRangeRect.height() / range;
        }
    }

    return beginning + offset;
}

MSliderViewPrivate::MSliderViewPrivate() :
    q_ptr(0),
    controller(0),
    sliderGroove(0),
    minIndicator(0),
    maxIndicator(0),
    horizontalPolicy(0),
    verticalPolicy(0),
    valueAnimation(0),
    pressTimerId(0),
    valueWhenPressed(0)
{
}

MSliderViewPrivate::~MSliderViewPrivate()
{
    sliderGroove->ensureSafeClosing();

    if (valueAnimation)
        delete valueAnimation;
}

//intializes main layout and layout policies
void MSliderViewPrivate::init(MSlider *controller)
{
    this->controller = controller;

    sliderGroove = new MSliderGroove(controller);
    sliderGroove->setObjectName("MSliderGroove");

    sliderGroove->init(controller);

    //these are minmax indicators
    minIndicator = new MSliderIndicator(controller);
    minIndicator->setObjectName("MSliderHandleMinMaxIndicator");

    maxIndicator = new MSliderIndicator(controller);
    maxIndicator->setObjectName("MSliderHandleMinMaxIndicator");

    bool reverse = qApp->isRightToLeft();

    MLayout *mainLayout = new MLayout;
    controller->setLayout(mainLayout);

    horizontalPolicy = new MLinearLayoutPolicy(mainLayout, Qt::Horizontal);
    horizontalPolicy->setSpacing(0);
    horizontalPolicy->setContentsMargins(0, 0, 0, 0);

    if (!reverse) {
        horizontalPolicy->addItem(minIndicator, Qt::AlignCenter);
        horizontalPolicy->addItem(sliderGroove, Qt::AlignCenter);
        horizontalPolicy->addItem(maxIndicator, Qt::AlignCenter);
    } else {
        horizontalPolicy->addItem(maxIndicator, Qt::AlignCenter);
        horizontalPolicy->addItem(sliderGroove, Qt::AlignCenter);
        horizontalPolicy->addItem(minIndicator, Qt::AlignCenter);
    }

    verticalPolicy = new MLinearLayoutPolicy(mainLayout, Qt::Vertical);
    verticalPolicy->setSpacing(0);
    verticalPolicy->setContentsMargins(0, 0, 0, 0);

    verticalPolicy->addItem(maxIndicator, Qt::AlignCenter);
    verticalPolicy->addItem(sliderGroove, Qt::AlignCenter);
    verticalPolicy->addItem(minIndicator, Qt::AlignCenter);
}

//changes slider orientation by activating
//the corresponding layout policy and setting
//corresponding min, max and preferred sizes
void MSliderViewPrivate::updateOrientation()
{
    MLayout *mainLayout = dynamic_cast<MLayout *>(controller->layout());
    if (!mainLayout || !horizontalPolicy || !verticalPolicy) {
        mWarning("MSliderView") << "MSlider was not initialized properly";
        return;
    }

    Q_Q(MSliderView);
    if (q->model()->orientation() == Qt::Horizontal) {
        if (mainLayout->policy() != horizontalPolicy)
            mainLayout->setPolicy(horizontalPolicy);
    }
    //in case of vertical sliders width and height
    //defined in css file will be simply swapped
    if (q->model()->orientation() == Qt::Vertical) {
        if (mainLayout->policy() != verticalPolicy)
            mainLayout->setPolicy(verticalPolicy);
    }

    sliderGroove->setOrientation(q->model()->orientation());
}

void MSliderViewPrivate::insertMinIndicatorToLayout()
{
    if (!horizontalPolicy || !verticalPolicy) {
        mWarning("MSliderView") << "MSlider was not initialized properly";
        return;
    }

    bool reverse = qApp->isRightToLeft();

    if (!reverse) {
        QGraphicsLayoutItem* firstItem = horizontalPolicy->itemAt(0);
        if (firstItem != minIndicator)
            horizontalPolicy->insertItem(0, minIndicator, Qt::AlignCenter);
    } else {
        QGraphicsLayoutItem* lastItem = horizontalPolicy->itemAt(horizontalPolicy->count() - 1);
        if (lastItem != minIndicator)
            horizontalPolicy->insertItem(horizontalPolicy->count(), minIndicator, Qt::AlignCenter);
    }

    QGraphicsLayoutItem* lastItem = verticalPolicy->itemAt(verticalPolicy->count() - 1);
    if (lastItem != minIndicator)
        verticalPolicy->insertItem(verticalPolicy->count(), minIndicator, Qt::AlignCenter);
}

void MSliderViewPrivate::insertMaxIndicatorToLayout()
{
    if (!horizontalPolicy || !verticalPolicy) {
        mWarning("MSliderView") << "MSlider was not initialized properly";
        return;
    }

    bool reverse = qApp->isRightToLeft();

    if (!reverse) {
        QGraphicsLayoutItem* lastItem = horizontalPolicy->itemAt(horizontalPolicy->count() - 1);
        if (lastItem != maxIndicator)
            horizontalPolicy->insertItem(horizontalPolicy->count(), maxIndicator, Qt::AlignCenter);
    } else {
        QGraphicsLayoutItem* firstItem = horizontalPolicy->itemAt(0);
        if (firstItem != maxIndicator)
            horizontalPolicy->insertItem(0, maxIndicator, Qt::AlignCenter);
    }

    QGraphicsLayoutItem* firstItem = verticalPolicy->itemAt(0);
    if (firstItem != maxIndicator)
        verticalPolicy->insertItem(0, maxIndicator, Qt::AlignCenter);
}

void MSliderViewPrivate::removeMinIndicatorFromLayout()
{
    if (!horizontalPolicy || !verticalPolicy) {
        mWarning("MSliderView") << "MSlider was not initialized properly";
        return;
    }

    bool reverse = qApp->isRightToLeft();

    if (!reverse) {
        QGraphicsLayoutItem* firstItem = horizontalPolicy->itemAt(0);
        if (firstItem == minIndicator)
            horizontalPolicy->removeAt(0);
    } else {
        QGraphicsLayoutItem* lastItem = horizontalPolicy->itemAt(horizontalPolicy->count() - 1);
        if (lastItem == minIndicator)
            horizontalPolicy->removeAt(horizontalPolicy->count() - 1);
    }

    QGraphicsLayoutItem* lastItem = verticalPolicy->itemAt(verticalPolicy->count() - 1);
    if (lastItem == minIndicator)
        verticalPolicy->removeAt(verticalPolicy->count() - 1);
}

void MSliderViewPrivate::removeMaxIndicatorFromLayout()
{
    if (!horizontalPolicy || !verticalPolicy) {
        mWarning("MSliderView") << "MSlider was not initialized properly";
        return;
    }

    bool reverse = qApp->isRightToLeft();

    if (!reverse) {
        QGraphicsLayoutItem* lastItem = horizontalPolicy->itemAt(horizontalPolicy->count() - 1);
        if (lastItem == maxIndicator)
            horizontalPolicy->removeAt(horizontalPolicy->count() - 1);
    } else {
        QGraphicsLayoutItem* firstItem = horizontalPolicy->itemAt(0);
        if (firstItem == maxIndicator)
            horizontalPolicy->removeAt(0);
    }

    QGraphicsLayoutItem* firstItem = verticalPolicy->itemAt(0);
    if (firstItem == maxIndicator)
        verticalPolicy->removeAt(0);
}

//returns true if user clicked on slider groove
//or slider handle area and false otherwise
bool MSliderViewPrivate::isCollision(QGraphicsSceneMouseEvent *event) const
{
    Q_Q(const MSliderView);
    QRectF clickableRect = sliderGroove->clickableArea();

    //there are some margins around the view
    //and those have to be considered
    QPointF groovePos = sliderGroove->pos();
    groovePos.setX(groovePos.x() - q->marginLeft());
    groovePos.setY(groovePos.y() - q->marginTop());

    clickableRect.translate(groovePos);

    QRectF clickableHandleRect = sliderGroove->clickableHandleArea();
    clickableHandleRect.translate(sliderGroove->pos());

    return (clickableRect.contains(event->pos()) || clickableHandleRect.contains(event->pos()));
}

//sets slider value to that one corresponding
//to mouse cursor position
void MSliderViewPrivate::updateValue(QGraphicsSceneMouseEvent *event)
{
    if (valueAnimation == 0) {
        valueAnimation = new QPropertyAnimation(controller, "value", controller);
        valueAnimation->setDuration(50);
        valueAnimation->setEasingCurve(QEasingCurve::OutSine);
    }

    //there are some margins around the view
    //and those have to be considered
    Q_Q(MSliderView);
    QPointF eventPos = event->pos();
    eventPos.setX(eventPos.x() + q->marginLeft());
    eventPos.setY(eventPos.y() + q->marginTop());

    valueAnimation->setEndValue(sliderGroove->screenPointToValue(eventPos));
    valueAnimation->start();
}

//refreshes slider groove (min, max and value, slider state)
void MSliderViewPrivate::updateSliderGroove()
{
    Q_Q(MSliderView);
    sliderGroove->setSliderValues(q->model()->minimum(), q->model()->maximum(), q->model()->value());
    sliderGroove->setSliderState(q->model()->state());
}

//refreshes slider seekbar related values (just for seekbars)
void MSliderViewPrivate::updateSeekBar()
{
    Q_Q(MSliderView);
    const MSeekBarModel *seekBarModel = qobject_cast<const MSeekBarModel *>(q->model());
    if (!seekBarModel)
        sliderGroove->setSeekBarValues(false);
    else
        sliderGroove->setSeekBarValues(true, seekBarModel->loadedContentMin(), seekBarModel->loadedContentMax());
}

MSliderView::MSliderView(MSlider *controller):
    MWidgetView(controller),
    d_ptr(new MSliderViewPrivate)
{
    Q_D(MSliderView);
    d->q_ptr = this;
    d->init(controller);

    connect(controller, SIGNAL(displayExited()), this, SLOT(lowerSliderHandleIndicator()));
}

MSliderView::~MSliderView()
{
    delete d_ptr;
}

void MSliderView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    Q_D(MSliderView);
    const char *member;

    foreach(member, modifications) {
        if (member == MSliderModel::Orientation) {
            d->updateOrientation();
            updateGeometry();
        }
        if (member == MSliderModel::State)
            d->updateSliderGroove();
        else if (member == MSliderModel::Minimum)
            d->updateSliderGroove();
        else if (member == MSliderModel::Maximum)
            d->updateSliderGroove();
        else if (member == MSliderModel::Value)
            d->updateSliderGroove();
        else if (member == MSliderModel::Steps)
            d->updateSliderGroove();
        else if (member == MSeekBarModel::LoadedContentMin)
            d->updateSeekBar();
        else if (member == MSeekBarModel::LoadedContentMax)
            d->updateSeekBar();
        else if (member == MSliderModel::MinLabelText)
            d->minIndicator->setText(model()->minLabelText());
        else if (member == MSliderModel::MaxLabelText)
            d->maxIndicator->setText(model()->maxLabelText());
        else if (member == MSliderModel::HandleLabelText)
            d->sliderGroove->setIndicatorText(model()->handleLabelText());
        else if (member == MSliderModel::MinLabelIcon)
            d->minIndicator->setImage(model()->minLabelIcon());
        else if (member == MSliderModel::MaxLabelIcon)
            d->maxIndicator->setImage(model()->maxLabelIcon());
        else if (member == MSliderModel::HandleLabelIcon)
            d->sliderGroove->setIndicatorImage(model()->handleLabelIcon());
        else if (member == MSliderModel::MinLabelVisible) {
            if (model()->minLabelVisible())
                d->insertMinIndicatorToLayout();
            else
                d->removeMinIndicatorFromLayout();
        } else if (member == MSliderModel::MaxLabelVisible) {
            if (model()->maxLabelVisible())
                d->insertMaxIndicatorToLayout();
            else
                d->removeMaxIndicatorFromLayout();
        }
    }
}

void MSliderView::setupModel()
{
    MWidgetView::setupModel();

    Q_D(MSliderView);

    d->minIndicator->setText(model()->minLabelText());
    d->maxIndicator->setText(model()->maxLabelText());
    d->sliderGroove->setIndicatorText(model()->handleLabelText());

    d->minIndicator->setImage(model()->minLabelIcon());
    d->maxIndicator->setImage(model()->maxLabelIcon());
    d->sliderGroove->setIndicatorImage(model()->handleLabelIcon());

    if (model()->minLabelVisible())
        d->insertMinIndicatorToLayout();
    else
        d->removeMinIndicatorFromLayout();

    if (model()->maxLabelVisible())
        d->insertMaxIndicatorToLayout();
    else
        d->removeMaxIndicatorFromLayout();

    d->sliderGroove->lowerHandleIndicator();

    d->updateOrientation();
    d->updateSliderGroove();
    d->updateSeekBar();

    updateGeometry();
}

void MSliderView::applyStyle()
{
    MWidgetView::applyStyle();

    Q_D(MSliderView);

    d->sliderGroove->setHandlePixmaps(style()->handlePixmap(),
                                      style()->handlePressedPixmap(),
                                      style()->handleVerticalPixmap(),
                                      style()->handleVerticalPressedPixmap());
    d->sliderGroove->setImages(style()->backgroundBaseImage(),
                               style()->backgroundElapsedImage(),
                               style()->backgroundReceivedImage(),
                               style()->backgroundVerticalBaseImage(),
                               style()->backgroundVerticalElapsedImage(),
                               style()->backgroundVerticalReceivedImage());

    d->sliderGroove->setGrooveThickness(style()->grooveThickness());
    d->sliderGroove->setGrooveLength(style()->groovePreferredLength(),
                                     style()->grooveMinimumLength(),
                                     style()->grooveMaximumLength());

    d->updateOrientation();

    //only to reposition slider handle
    d->updateSliderGroove();

    updateGeometry();
}

void MSliderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MSliderView);
    if (d->isCollision(event)) {
        d->valueWhenPressed = model()->value();

        d->controller->setState(MSliderModel::Pressed);

        d->updateValue(event);
        d->pressTimerId = startTimer(100);
    }
}

void MSliderView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MSliderView);
    d->controller->setState(MSliderModel::Released);

    if (d->pressTimerId) {
        killTimer(d->pressTimerId);
        d->pressTimerId = 0;
    }

    d->valueAnimation->stop();
    model()->setValue(d->valueWhenPressed);
    d->sliderGroove->lowerHandleIndicator();
}

void MSliderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MSliderView);

    d->controller->setState(MSliderModel::Released);
    d->updateValue(event);

    if (d->pressTimerId) {
        killTimer(d->pressTimerId);
        d->pressTimerId = 0;
    }

    d->sliderGroove->lowerHandleIndicator();
}

void MSliderView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MSliderView);
    if (d->isCollision(event)) {
        d->controller->setState(MSliderModel::Pressed);
        d->updateValue(event);

        if (model()->handleLabelVisible())
            d->sliderGroove->raiseHandleIndicator();
    } else {
        d->controller->setState(MSliderModel::Released);
        d->updateValue(event);
        d->sliderGroove->lowerHandleIndicator();
    }
}

void MSliderView::timerEvent(QTimerEvent *event)
{
    Q_D(MSliderView);

    if (event->timerId() == d->pressTimerId) {
        killTimer(d->pressTimerId);
        d->pressTimerId = 0;

        if (model()->handleLabelVisible())
            d->sliderGroove->raiseHandleIndicator();
    }
}

void MSliderView::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);

    lowerSliderHandleIndicator();
}

void MSliderView::lowerSliderHandleIndicator()
{
    Q_D(MSliderView);
 
    d->controller->setState(MSliderModel::Released);
 
    if (d->pressTimerId) {
        killTimer(d->pressTimerId);
        d->pressTimerId = 0;
    }
 
    d->sliderGroove->lowerHandleIndicator();
}

M_REGISTER_VIEW_NEW(MSliderView, MSlider)
