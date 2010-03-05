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

#include "duisliderview.h"
#include "duisliderview_p.h"

#include <math.h>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QSizeF>
#include <QApplication>
#include <QGraphicsAnchorLayout>
#include <QGraphicsGridLayout>
#include <QPropertyAnimation>
#include <limits>

#include "duitheme.h"
#include "duiscalableimage.h"
#include "duiwidgetview.h"
#include "duislider.h"
#include "duiseekbar.h"
#include "duislider_p.h"
#include "duiviewcreator.h"
#include "duilabel.h"
#include "duiimagewidget.h"
#include "duilayout.h"
#include "duilinearlayoutpolicy.h"

DuiSliderHandle::DuiSliderHandle(QGraphicsItem *parent) :
    DuiWidget(parent),
    orientation(Qt::Horizontal),
    handlePixmap(0),
    handlePressedPixmap(0),
    handleVerticalPixmap(0),
    handleVerticalPressedPixmap(0),
    sliderState(DuiSliderModel::Released)
{
}

DuiSliderHandle::~DuiSliderHandle()
{
}

void DuiSliderHandle::setOrientation(Qt::Orientation orientation)
{
    this->orientation = orientation;
    updateGeometry();
}

void DuiSliderHandle::setPixmaps(const QPixmap *handle,
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

void DuiSliderHandle::setSliderState(DuiSliderModel::SliderState state)
{
    sliderState = state;
    updateGeometry();
}

void DuiSliderHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRect handleRect = rect().toRect();

    if (sliderState == DuiSliderModel::Pressed) {
        if (orientation == Qt::Horizontal)
            painter->drawPixmap((handleRect.width() - handlePressedPixmap->width()) / 2,
                                (handleRect.height() - handlePressedPixmap->height()) / 2,
                                *handlePressedPixmap);
        if (orientation == Qt::Vertical)
            painter->drawPixmap((handleRect.width() - handleVerticalPressedPixmap->width()) / 2,
                                (handleRect.height() - handleVerticalPressedPixmap->height()) / 2,
                                *handleVerticalPressedPixmap);
    }
    if (sliderState == DuiSliderModel::Released) {
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

QSizeF DuiSliderHandle::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    int width = 0;
    int height = 0;

    if (orientation == Qt::Horizontal) {
        if (sliderState == DuiSliderModel::Released) {
            if (handlePixmap) {
                width = qMax(width, handlePixmap->width());
                height = qMax(height, handlePixmap->height());
            }
        }

        if (sliderState == DuiSliderModel::Pressed) {
            if (handlePressedPixmap) {
                width = qMax(width, handlePressedPixmap->width());
                height = qMax(height, handlePressedPixmap->height());
            }
        }
    }

    if (orientation == Qt::Vertical) {
        if (sliderState == DuiSliderModel::Released) {
            if (handleVerticalPixmap) {
                width = qMax(width, handleVerticalPixmap->width());
                height = qMax(height, handleVerticalPixmap->height());
            }
        }

        if (sliderState == DuiSliderModel::Pressed) {
            if (handleVerticalPressedPixmap) {
                width = qMax(width, handleVerticalPressedPixmap->width());
                height = qMax(height, handleVerticalPressedPixmap->height());
            }
        }
    }

    return QSizeF(width, height);
}

DuiSliderIndicator::DuiSliderIndicator(bool isMinMax, QGraphicsItem *parent) :
    DuiWidget(parent),
    label(0),
    image(0),
    visibility(false),
    layout(0)
{
    layout = new QGraphicsAnchorLayout;
    setLayout(layout);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    label = new DuiLabel;
    if (isMinMax)
        label->setObjectName("DuiSliderMinMaxLabel");
    else
        label->setObjectName("DuiSliderHandleLabel");

    image = new DuiImageWidget;
    image->setObjectName("DuiSliderImage");

    layout->addAnchor(layout, Qt::AnchorVerticalCenter, label, Qt::AnchorVerticalCenter);
    layout->addAnchor(layout, Qt::AnchorVerticalCenter, image, Qt::AnchorVerticalCenter);

    label->resize(0, 0);
    image->resize(0, 0);
}

DuiSliderIndicator::~DuiSliderIndicator()
{
}

void DuiSliderIndicator::setText(const QString &text)
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

void DuiSliderIndicator::setImage(const QString &id)
{
    image->setVisible(!id.isEmpty());

    imageName = id;

    QPixmap *pixmap = 0;
    if (!id.isEmpty()) {
        pixmap = DuiTheme::pixmapCopy(id);
        image->setPixmap(*pixmap);
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

void DuiSliderIndicator::setVisibility(bool visibility)
{
    this->visibility = visibility;
    DuiWidget::setVisible(visibility);
    updateGeometry();
}

QSizeF DuiSliderIndicator::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    qreal width = 0;
    qreal height = 0;

    if (visibility) {
        if (image && !imageName.isEmpty()) {
            width = qMax(width, image->sizeHint(Qt::PreferredSize).width());
            height = qMax(height, image->sizeHint(Qt::PreferredSize).height());
        }

        if (label && !label->text().isEmpty()) {
            width = qMax(width, label->sizeHint(Qt::PreferredSize).width());
            height = qMax(height, label->sizeHint(Qt::PreferredSize).height());
        }  
    }

    return QSizeF(width, height);
}

DuiSliderGroove::DuiSliderGroove(QGraphicsItem *parent) :
    DuiWidget(parent),
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
    sliderHandle = new DuiSliderHandle(this);
    sliderHandleIndicator = new DuiSliderIndicator(false, this);
}

DuiSliderGroove::~DuiSliderGroove()
{
    if (backgroundBaseImage)
        DuiTheme::releaseScalableImage(backgroundBaseImage);
    if (backgroundElapsedImage)
        DuiTheme::releaseScalableImage(backgroundElapsedImage);
    if (backgroundReceivedImage)
        DuiTheme::releaseScalableImage(backgroundReceivedImage);
    if (backgroundVerticalBaseImage)
        DuiTheme::releaseScalableImage(backgroundVerticalBaseImage);
    if (backgroundVerticalElapsedImage)
        DuiTheme::releaseScalableImage(backgroundVerticalElapsedImage);
    if (backgroundVerticalReceivedImage)
        DuiTheme::releaseScalableImage(backgroundVerticalReceivedImage);

    ensureSafeClosing();
}

void DuiSliderGroove::init(DuiSlider *controller)
{
    this->controller = controller;
}

void DuiSliderGroove::setOrientation(Qt::Orientation orientation)
{
    this->orientation = orientation;
    sliderHandle->setOrientation(orientation);

    updateGeometry();
}

void DuiSliderGroove::setHandlePixmaps(const QPixmap *handle,
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

void DuiSliderGroove::setImages(const DuiScalableImage *background,
                                const DuiScalableImage *backgroundElapsed,
                                const DuiScalableImage *backgroundReceived,
                                const DuiScalableImage *backgroundVertical,
                                const DuiScalableImage *backgroundVerticalElapsed,
                                const DuiScalableImage *backgroundVerticalReceived)
{
    backgroundBaseImage = background;
    backgroundElapsedImage = backgroundElapsed;
    backgroundReceivedImage = backgroundReceived;
    backgroundVerticalBaseImage = backgroundVertical;
    backgroundVerticalElapsedImage = backgroundVerticalElapsed;
    backgroundVerticalReceivedImage = backgroundVerticalReceived;

    updateGeometry();
}

void DuiSliderGroove::setGrooveThickness(qreal thickness)
{
    grooveThickness = thickness;

    updateGeometry();
}

void DuiSliderGroove::setGrooveLength(qreal prefLength, qreal minLength, qreal maxLength)
{
    preferredLength = prefLength;
    minimumLength = minLength;
    maximumLength = maxLength;

    updateGeometry();
}

void DuiSliderGroove::setSliderValues(int min, int max, int val)
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

void DuiSliderGroove::setSeekBarValues(bool show, int loadedContentMin, int loadedContentMax)
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

void DuiSliderGroove::setSliderState(DuiSliderModel::SliderState state)
{
    sliderHandle->setSliderState(state);
    updateHandleIndicatorPos();
}

void DuiSliderGroove::setIndicatorText(const QString &text)
{
    sliderHandleIndicator->setText(text);
    updateHandleIndicatorPos();
}

void DuiSliderGroove::setIndicatorImage(const QString &id)
{
    sliderHandleIndicator->setImage(id);
    updateHandleIndicatorPos();
}

void DuiSliderGroove::setIndicatorVisibility(bool visibility)
{
    sliderHandleIndicator->setVisibility(visibility);
    updateHandleIndicatorPos();
}

//converts one of coordinates of point to slider value
int DuiSliderGroove::screenPointToValue(const QPointF &point) const
{
    bool reverse = qApp->isRightToLeft();
    QPointF handlePoint = point - pos();

    qreal coordinate = 0;

    if (orientation == Qt::Horizontal)
        coordinate = handlePoint.x();
    if (orientation == Qt::Vertical)
        coordinate = handlePoint.y();

    int offset = 0;

    if (minimum != maximum) {
        QRectF clickableRect = clickableArea();

        if (orientation == Qt::Horizontal) {
            coordinate = qBound(clickableRect.left(), coordinate, clickableRect.right());

            if (!reverse)
                offset = qRound(((coordinate - clickableRect.left()) * (maximum - minimum)) / clickableRect.width());
            else
                offset = qRound(((clickableRect.right() - coordinate) * (maximum - minimum)) / clickableRect.width());
        }

        if (orientation == Qt::Vertical) {
            coordinate = qBound(clickableRect.top(), coordinate, clickableRect.bottom());
            offset = qRound(((clickableRect.bottom() - coordinate) * (maximum - minimum)) / clickableRect.height());
        }
    }

    return minimum + offset;
}

//determines clickable area which is basically the
//exact area of slider groove
QRectF DuiSliderGroove::clickableArea() const
{
    QRectF grooveRect = rect();
    if (!grooveRect.isValid())
        return grooveRect;

    if (orientation == Qt::Horizontal) {
        qreal hLeftAdjustment = sliderHandle->rect().width() / 2;
        qreal hRightAdjustment = hLeftAdjustment;

        if (backgroundBaseImage) {
            int left, right, top, bottom;
            backgroundBaseImage->borders(&left, &right, &top, &bottom);

            hLeftAdjustment = qMax(hLeftAdjustment, qreal(left));
            hRightAdjustment = qMax(hRightAdjustment, qreal(right));
        }
        grooveRect.adjust(hLeftAdjustment, 0, -hRightAdjustment, 0);
    }

    if (orientation == Qt::Vertical) {
        qreal vTopAdjustment = sliderHandle->rect().height() / 2;
        qreal vBottomAdjustment = vTopAdjustment;

        if (backgroundVerticalBaseImage) {
            int left, right, top, bottom;
            backgroundVerticalBaseImage->borders(&left, &right, &top, &bottom);

            vTopAdjustment = qMax(vTopAdjustment, qreal(top));
            vBottomAdjustment = qMax(vBottomAdjustment, qreal(bottom));
        }
        grooveRect.adjust(0, vTopAdjustment, 0, -vBottomAdjustment);
    }

    return grooveRect;
}

//area occupied by slider handle (user can)
//click both of them
QRectF DuiSliderGroove::clickableHandleArea() const
{
    QRectF handleRect = sliderHandle->rect();

    handleRect.translate(sliderHandle->pos());
    return handleRect;
}

QRectF DuiSliderGroove::boundingRect() const
{
    return sliderBoundingRect;
}

void DuiSliderGroove::raiseHandleIndicator()
{
    QGraphicsItem *newParent = topLevelItem();
    if (newParent)
        sliderHandleIndicator->setParentItem(newParent);

    updateHandleIndicatorPos();
}

void DuiSliderGroove::lowerHandleIndicator()
{
    sliderHandleIndicator->setParentItem(this);
    updateHandleIndicatorPos();
}

void DuiSliderGroove::ensureSafeClosing()
{
    sliderHandleIndicator->setParentItem(this);
}

void DuiSliderGroove::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    bool reverse = qApp->isRightToLeft();

    QRectF grooveRect = rect();
    if (!grooveRect.isValid())
        return;

    int left, right, top, bottom;

    if (orientation == Qt::Horizontal) {
        qreal hLeftAdjustment = 0;
        qreal hRightAdjustment = 0;

        if (backgroundBaseImage) {
            backgroundBaseImage->borders(&left, &right, &top, &bottom);

            qreal semiWidth = sliderHandle->rect().width() / 2;

            if (semiWidth > left)
                hLeftAdjustment = semiWidth;
            if (semiWidth > right)
                hRightAdjustment = semiWidth;
        }

        qreal vAdjustment = (grooveRect.height() - grooveThickness) / 2;
        grooveRect.adjust(hLeftAdjustment, vAdjustment, -hRightAdjustment, -vAdjustment);

        if (backgroundBaseImage) {
            if (grooveRect.width() >= qreal(backgroundBaseImage->pixmap()->width()))
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

                if (receivedRect.width() >= qreal(backgroundReceivedImage->pixmap()->width()))
                    backgroundReceivedImage->draw(receivedRect.toRect(), painter);
            }
        }

        if (backgroundElapsedImage) {
            QRectF elapsedRect = grooveRect;

            if (!reverse)
                elapsedRect.setRight(valueToScreenCoordinate(value));
            else
                elapsedRect.setLeft(valueToScreenCoordinate(value));

            if (elapsedRect.width() >= qreal(backgroundElapsedImage->pixmap()->width()))
                backgroundElapsedImage->draw(elapsedRect.toRect(), painter);
        }
    }

    if (orientation == Qt::Vertical) {
        qreal vTopAdjustment = 0;
        qreal vBottomAdjustment = 0;

        if (backgroundVerticalBaseImage) {
            backgroundVerticalBaseImage->borders(&left, &right, &top, &bottom);

            qreal semiHeight = sliderHandle->rect().height() / 2;

            if (semiHeight > top)
                vTopAdjustment = semiHeight;
            if (semiHeight > bottom)
                vBottomAdjustment = semiHeight;
        }

        qreal hAdjustment = (grooveRect.width() - grooveThickness) / 2;
        grooveRect.adjust(hAdjustment, vTopAdjustment, -hAdjustment, -vBottomAdjustment);

        if (backgroundVerticalBaseImage) {
            if (grooveRect.height() >= qreal(backgroundVerticalBaseImage->pixmap()->height()))
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

                if (receivedRect.height() >= qreal(backgroundVerticalReceivedImage->pixmap()->height()))
                    backgroundVerticalReceivedImage->draw(receivedRect.toRect(), painter);
            }
        }

        if (backgroundVerticalElapsedImage) {
            QRectF elapsedRect = grooveRect;

            elapsedRect.setTop(valueToScreenCoordinate(value));

            if (elapsedRect.height() >= qreal(backgroundVerticalElapsedImage->pixmap()->height()))
                backgroundVerticalElapsedImage->draw(elapsedRect.toRect(), painter);
        }
    }
}

//repositions slider handle (and slider handle indicator)
void DuiSliderGroove::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);

    QPointF handlePos;

    if (orientation == Qt::Horizontal)
        handlePos.setX(valueToScreenCoordinate(value));
    if (orientation == Qt::Vertical)
        handlePos.setY(valueToScreenCoordinate(value));

    updateHandlePos(handlePos);
}

QSizeF DuiSliderGroove::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);

    switch (which) {
    case Qt::MinimumSize: {
        if (orientation == Qt::Horizontal)
            return QSizeF(minimumLength, sliderHandle->sizeHint(Qt::PreferredSize).height());
        if (orientation == Qt::Vertical)
            return QSizeF(sliderHandle->sizeHint(Qt::PreferredSize).width(), minimumLength);
            //return QSizeF(0, 0);
    }
    case Qt::PreferredSize: {
        if (orientation == Qt::Horizontal)
            return QSizeF(preferredLength, sliderHandle->sizeHint(Qt::PreferredSize).height());
        if (orientation == Qt::Vertical)
            return QSizeF(sliderHandle->sizeHint(Qt::PreferredSize).width(), preferredLength);
        //return QSizeF(0, 0);
    }
    case Qt::MaximumSize: {
        if (orientation == Qt::Horizontal)
            return QSizeF(maximumLength, sliderHandle->sizeHint(Qt::PreferredSize).height());
        if (orientation == Qt::Vertical)
            return QSizeF(sliderHandle->sizeHint(Qt::PreferredSize).width(), maximumLength);
        //return QSizeF(0, 0);
    }
    default:
        qWarning("DuiSliderView::sizeHint() don't know how to handle the value of 'which' ");
    }

    return QSizeF(0, 0);
}

//called when slider groove is repositioned or
//resized (overidden to reposition slider handle
//(and slider handle indicator))
void DuiSliderGroove::setGeometry(const QRectF &rect)
{
    DuiWidget::setGeometry(rect);

    setSliderValues(minimum, maximum, value);
    setSeekBarValues(showSeekBar, loadedContentMinimum, loadedContentMaximum);
}

//sets middle point so that the whole rect occupied
//by handle remains inside rect occupied by groove
void DuiSliderGroove::updateHandlePos(const QPointF &position)
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
void DuiSliderGroove::updateHandleIndicatorPos()
{
    if (!controller)
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
qreal DuiSliderGroove::valueToScreenCoordinate(int value) const
{
    bool reverse = qApp->isRightToLeft();
    qreal beginning = 0;

    if (orientation == Qt::Horizontal)
        beginning = clickableArea().left();
    if (orientation == Qt::Vertical)
        beginning = clickableArea().top();

    qreal offset = 0;
    if (minimum != maximum) {
        if (orientation == Qt::Horizontal) {
            if (!reverse)
                offset = (value - minimum) * clickableArea().width() / (maximum - minimum);
            else
                offset = (maximum - value) * clickableArea().width() / (maximum - minimum);
        }

        if (orientation == Qt::Vertical) {
            offset = (maximum - value) * clickableArea().height() / (maximum - minimum);
        }
    }

    return beginning + offset;
}

DuiSliderViewPrivate::DuiSliderViewPrivate() :
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
    sliderGroove = new DuiSliderGroove;
    //these are minmax indicators
    minIndicator = new DuiSliderIndicator(true);
    maxIndicator = new DuiSliderIndicator(true);
}

DuiSliderViewPrivate::~DuiSliderViewPrivate()
{
    sliderGroove->ensureSafeClosing();

    if (valueAnimation)
        delete valueAnimation;
}

//intializes main layout and layout policies
void DuiSliderViewPrivate::init(DuiSlider *controller)
{
    this->controller = controller;
    sliderGroove->init(controller);

    bool reverse = qApp->isRightToLeft();

    DuiLayout *mainLayout = new DuiLayout;
    controller->setLayout(mainLayout);

    horizontalPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Horizontal);
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

    verticalPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    verticalPolicy->setSpacing(0);
    verticalPolicy->setContentsMargins(0, 0, 0, 0);

    verticalPolicy->addItem(maxIndicator, Qt::AlignCenter);
    verticalPolicy->addItem(sliderGroove, Qt::AlignCenter);
    verticalPolicy->addItem(minIndicator, Qt::AlignCenter);
}

//changes slider orientation by activating
//the corresponding layout policy and setting
//corresponding min, max and preferred sizes
void DuiSliderViewPrivate::updateOrientation()
{
    DuiLayout *mainLayout = dynamic_cast<DuiLayout *>(controller->layout());
    if (!mainLayout || !horizontalPolicy || !verticalPolicy) {
        qWarning("DuiSlider was not initialized properly");
        return;
    }

    Q_Q(DuiSliderView);
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

//returns true if user clicked on slider groove
//or slider handle area and false otherwise
bool DuiSliderViewPrivate::isCollision(QGraphicsSceneMouseEvent *event) const
{
    Q_Q(const DuiSliderView);
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
void DuiSliderViewPrivate::updateValue(QGraphicsSceneMouseEvent *event)
{
    if (valueAnimation == 0) {
        valueAnimation = new QPropertyAnimation(controller, "value", controller);
        valueAnimation->setDuration(150);
        valueAnimation->setEasingCurve(QEasingCurve::OutSine);
    }

    //there are some margins around the view
    //and those have to be considered
    Q_Q(DuiSliderView);
    QPointF eventPos = event->pos();
    eventPos.setX(eventPos.x() + q->marginLeft());
    eventPos.setY(eventPos.y() + q->marginTop());

    valueAnimation->setEndValue(sliderGroove->screenPointToValue(eventPos));
    valueAnimation->start();
}

//refreshes slider groove (min, max and value, slider state)
void DuiSliderViewPrivate::updateSliderGroove()
{
    Q_Q(DuiSliderView);
    sliderGroove->setSliderValues(q->model()->minimum(), q->model()->maximum(), q->model()->value());
    sliderGroove->setSliderState(q->model()->state());
}

//refreshes slider seekbar related values (just for seekbars)
void DuiSliderViewPrivate::updateSeekBar()
{
    Q_Q(DuiSliderView);
    const DuiSeekBarModel *seekBarModel = qobject_cast<const DuiSeekBarModel *>(q->model());
    if (!seekBarModel)
        sliderGroove->setSeekBarValues(false);
    else
        sliderGroove->setSeekBarValues(true, seekBarModel->loadedContentMin(), seekBarModel->loadedContentMax());
}

DuiSliderView::DuiSliderView(DuiSlider *controller):
    DuiWidgetView(*new DuiSliderViewPrivate, controller)
{
    Q_D(DuiSliderView);
    d->init(controller);
}

DuiSliderView::~DuiSliderView()
{
}

void DuiSliderView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);

    Q_D(DuiSliderView);
    const char *member;

    foreach(member, modifications) {
        if (member == DuiSliderModel::Orientation) {
            d->updateOrientation();
            updateGeometry();
        }
        if (member == DuiSliderModel::State)
            d->updateSliderGroove();
        else if (member == DuiSliderModel::Minimum)
            d->updateSliderGroove();
        else if (member == DuiSliderModel::Maximum)
            d->updateSliderGroove();
        else if (member == DuiSliderModel::Value)
            d->updateSliderGroove();
        else if (member == DuiSliderModel::Steps)
            d->updateSliderGroove();
        else if (member == DuiSeekBarModel::LoadedContentMin)
            d->updateSeekBar();
        else if (member == DuiSeekBarModel::LoadedContentMax)
            d->updateSeekBar();
        else if (member == DuiSliderModel::MinLabelText)
            d->minIndicator->setText(model()->minLabelText());
        else if (member == DuiSliderModel::MaxLabelText)
            d->maxIndicator->setText(model()->maxLabelText());
        else if (member == DuiSliderModel::HandleLabelText)
            d->sliderGroove->setIndicatorText(model()->handleLabelText());
        else if (member == DuiSliderModel::MinLabelIcon)
            d->minIndicator->setImage(model()->minLabelIcon());
        else if (member == DuiSliderModel::MaxLabelIcon)
            d->maxIndicator->setImage(model()->maxLabelIcon());
        else if (member == DuiSliderModel::HandleLabelIcon)
            d->sliderGroove->setIndicatorImage(model()->handleLabelIcon());
        else if (member == DuiSliderModel::MinLabelVisible)
            d->minIndicator->setVisibility(model()->minLabelVisible());
        else if (member == DuiSliderModel::MaxLabelVisible)
            d->maxIndicator->setVisibility(model()->maxLabelVisible());
        else if (member == DuiSliderModel::HandleLabelVisible)
            d->sliderGroove->setIndicatorVisibility(model()->handleLabelVisible());
    }
}

void DuiSliderView::setupModel()
{
    DuiWidgetView::setupModel();

    Q_D(DuiSliderView);

    d->minIndicator->setText(model()->minLabelText());
    d->maxIndicator->setText(model()->maxLabelText());
    d->sliderGroove->setIndicatorText(model()->handleLabelText());

    d->minIndicator->setImage(model()->minLabelIcon());
    d->maxIndicator->setImage(model()->maxLabelIcon());
    d->sliderGroove->setIndicatorImage(model()->handleLabelIcon());

    d->minIndicator->setVisibility(model()->minLabelVisible());
    d->maxIndicator->setVisibility(model()->maxLabelVisible());
    d->sliderGroove->setIndicatorVisibility(model()->handleLabelVisible());

    d->updateOrientation();
    d->updateSliderGroove();
    d->updateSeekBar();

    updateGeometry();
}

void DuiSliderView::applyStyle()
{
    DuiWidgetView::applyStyle();

    Q_D(DuiSliderView);

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

void DuiSliderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiSliderView);
    if (d->isCollision(event)) {
        d->valueWhenPressed = model()->value();

        d->updateValue(event);
        d->pressTimerId = startTimer(100);
    }
}

void DuiSliderView::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(DuiSliderView);
    d->controller->setState(DuiSliderModel::Released);


    if (d->pressTimerId) {
        killTimer(d->pressTimerId);
        d->pressTimerId = 0;
    }

    d->valueAnimation->stop();
    model()->setValue(d->valueWhenPressed);
    d->sliderGroove->lowerHandleIndicator();
}

void DuiSliderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiSliderView);
    if (d->isCollision(event) || model()->state() == DuiSliderModel::Pressed) {
        d->controller->setState(DuiSliderModel::Released);
        d->updateValue(event);

        if (d->pressTimerId) {
            killTimer(d->pressTimerId);
            d->pressTimerId = 0;
        }

        d->sliderGroove->lowerHandleIndicator();
    }
}

void DuiSliderView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiSliderView);
    if (d->isCollision(event)) {
        d->controller->setState(DuiSliderModel::Pressed);
        d->updateValue(event);
        d->sliderGroove->raiseHandleIndicator();
    } else if (model()->state() == DuiSliderModel::Pressed) {
        d->controller->setState(DuiSliderModel::Released);
        d->updateValue(event);
        d->sliderGroove->lowerHandleIndicator();
    }
}

void DuiSliderView::timerEvent(QTimerEvent *event)
{
    Q_D(DuiSliderView);

    if (event->timerId() == d->pressTimerId) {
        killTimer(d->pressTimerId);
        d->pressTimerId = 0;

        d->controller->setState(DuiSliderModel::Pressed);
        d->sliderGroove->raiseHandleIndicator();
    }
}

void DuiSliderView::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);

    Q_D(DuiSliderView);
    if (d->pressTimerId) {
        killTimer(d->pressTimerId);
        d->pressTimerId = 0;
    }

    d->sliderGroove->lowerHandleIndicator();
}

DUI_REGISTER_VIEW_NEW(DuiSliderView, DuiSlider)
