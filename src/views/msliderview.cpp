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
#include <QTimer>
#include <MDeviceProfile>
#include <limits>
#include <QSwipeGesture>

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
        if (orientation == Qt::Horizontal && handlePressedPixmap != 0)
            painter->drawPixmap((handleRect.width() - handlePressedPixmap->width()) / 2,
                                (handleRect.height() - handlePressedPixmap->height()) / 2,
                                *handlePressedPixmap);
        if (orientation == Qt::Vertical && handleVerticalPressedPixmap != 0)
            painter->drawPixmap((handleRect.width() - handleVerticalPressedPixmap->width()) / 2,
                                (handleRect.height() - handleVerticalPressedPixmap->height()) / 2,
                                *handleVerticalPressedPixmap);
    }
    if (sliderState == MSliderModel::Released) {
        if (orientation == Qt::Horizontal && handlePixmap != 0)
            painter->drawPixmap((handleRect.width() - handlePixmap->width()) / 2,
                                (handleRect.height() - handlePixmap->height()) / 2,
                                *handlePixmap);
        if (orientation == Qt::Vertical && handleVerticalPixmap != 0)
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

MSliderHandleIndicatorArrow::MSliderHandleIndicatorArrow(QGraphicsItem *parent) :
    MWidget(parent),
    orientation(Qt::Horizontal),
    handleLabelArrowLeftPixmap(0),
    handleLabelArrowRightPixmap(0),
    handleLabelArrowUpPixmap(0),
    handleLabelArrowDownPixmap(0)
{
}

MSliderHandleIndicatorArrow::~MSliderHandleIndicatorArrow()
{
}

void MSliderHandleIndicatorArrow::setOrientation(Qt::Orientation orientation)
{
    this->orientation = orientation;
    updateGeometry();
}

void MSliderHandleIndicatorArrow::setPixmaps(const QPixmap *handleLabelArrowLeft,
                                             const QPixmap *handleLabelArrowRight,
                                             const QPixmap *handleLabelArrowUp,
                                             const QPixmap *handleLabelArrowDown)
{
    handleLabelArrowLeftPixmap = handleLabelArrowLeft;
    handleLabelArrowRightPixmap = handleLabelArrowRight;
    handleLabelArrowUpPixmap = handleLabelArrowUp;
    handleLabelArrowDownPixmap = handleLabelArrowDown;

    updateGeometry();
}

void MSliderHandleIndicatorArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    bool reverse = qApp->isRightToLeft();

    QRect handleRect = rect().toRect();
    if (orientation == Qt::Horizontal && handleLabelArrowDownPixmap != 0)
        painter->drawPixmap((handleRect.width() - handleLabelArrowDownPixmap->width()) / 2,
                            (handleRect.height() - handleLabelArrowDownPixmap->height()) / 2,
                            *handleLabelArrowDownPixmap);
     if (orientation == Qt::Vertical) {
         if (!reverse) {
             if (handleLabelArrowLeftPixmap != 0)
                 painter->drawPixmap((handleRect.width() - handleLabelArrowLeftPixmap->width()) / 2,
                                     (handleRect.height() - handleLabelArrowLeftPixmap->height()) / 2,
                                     *handleLabelArrowLeftPixmap);
         } else {
             if (handleLabelArrowRightPixmap != 0)
                 painter->drawPixmap((handleRect.width() - handleLabelArrowRightPixmap->width()) / 2,
                                     (handleRect.height() - handleLabelArrowRightPixmap->height()) / 2,
                                     *handleLabelArrowRightPixmap);
         }
    }
}

QSizeF MSliderHandleIndicatorArrow::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    bool reverse = qApp->isRightToLeft();

    if (orientation == Qt::Horizontal) {
        if (handleLabelArrowDownPixmap)
            return QSizeF(handleLabelArrowDownPixmap->width(), handleLabelArrowDownPixmap->height());
    }

    if (orientation == Qt::Vertical) {
        if (!reverse) {
            if (handleLabelArrowLeftPixmap)
                return QSizeF(handleLabelArrowLeftPixmap->width(), handleLabelArrowLeftPixmap->height());
        } else {
            if (handleLabelArrowRightPixmap)
                return QSizeF(handleLabelArrowRightPixmap->width(), handleLabelArrowRightPixmap->height());
        }
    }

    return QSizeF(0, 0);
}

MSliderIndicator::MSliderIndicator(bool isMinMax, QGraphicsItem *parent) :
    MWidget(parent),
    label(0),
    image(0),
    fixedWidth(0),
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
    layout->addAnchor(layout, Qt::AnchorHorizontalCenter, label, Qt::AnchorHorizontalCenter);
    layout->addAnchor(layout, Qt::AnchorVerticalCenter, image, Qt::AnchorVerticalCenter);

    label->resize(0, 0);
    image->resize(0, 0);
}

MSliderIndicator::~MSliderIndicator()
{
}

void MSliderIndicator::setLabelStyleName(const QString &stylename)
{
    label->setStyleName(stylename);
}

void MSliderIndicator::setText(const QString &text)
{
    bool sizeHintChanged = false;
    label->setVisible(!text.isEmpty());

    label->setText(text);

    if (text.isEmpty()) {
        QSizeF emptySize(0, 0);

        if (emptySize != label->minimumSize()) {
            label->setMinimumSize(0, 0);
            sizeHintChanged = true;
        }
        if (emptySize != label->preferredSize()) {
            label->setPreferredSize(0, 0);
            sizeHintChanged = true;
        }
        if (emptySize != label->maximumSize()) {
            label->setMaximumSize(0, 0);
            sizeHintChanged = true;
        }
    } else {
        if (label->sizeHint(Qt::PreferredSize) != label->minimumSize()) {
            label->setMinimumSize(label->sizeHint(Qt::PreferredSize));
            sizeHintChanged = true;
        }
        if (label->sizeHint(Qt::PreferredSize) != label->preferredSize()) {
            label->setPreferredSize(label->sizeHint(Qt::PreferredSize));
            sizeHintChanged = true;
        }
        if (label->sizeHint(Qt::PreferredSize) != label->maximumSize()) {
            label->setMaximumSize(label->sizeHint(Qt::PreferredSize));
            sizeHintChanged = true;
        }
    }

    if (label->sizeHint(Qt::PreferredSize) != label->size())
        label->resize(label->sizeHint(Qt::PreferredSize));

    if (sizeHintChanged)
        updateGeometry();
}

void MSliderIndicator::setImage(const QString &id)
{
    bool sizeHintChanged = false;
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

    if (id.isEmpty()) {
        QSizeF emptySize(0, 0);

        if (emptySize != image->minimumSize()) {
            image->setMinimumSize(0, 0);
            sizeHintChanged = true;
        }
        if (emptySize != image->preferredSize()) {
            image->setPreferredSize(0, 0);
            sizeHintChanged = true;
        }
        if (emptySize != image->maximumSize()) {
            image->setMaximumSize(0, 0);
            sizeHintChanged = true;
        }
    } else {
        if (image->sizeHint(Qt::PreferredSize) != image->minimumSize()) {
            image->setMinimumSize(image->sizeHint(Qt::PreferredSize));
            sizeHintChanged = true;
        }
        if (image->sizeHint(Qt::PreferredSize) != image->preferredSize()) {
            image->setPreferredSize(image->sizeHint(Qt::PreferredSize));
            sizeHintChanged = true;
        }
        if (image->sizeHint(Qt::PreferredSize) != image->maximumSize()) {
            image->setMaximumSize(image->sizeHint(Qt::PreferredSize));
            sizeHintChanged = true;
        }
    }

    if (image->sizeHint(Qt::PreferredSize) != label->size())
        image->resize(image->sizeHint(Qt::PreferredSize));

    if (sizeHintChanged)
        updateGeometry();
}

bool MSliderIndicator::isEmpty() const
{
    if (label->text().isEmpty() && image->imageId().isEmpty())
        return true;

    return false;
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
        if (fixedWidth) {
            width = fixedWidth;
        } else {
            width = qMax(width, label->sizeHint(Qt::PreferredSize).width());
        }
        height = qMax(height, label->sizeHint(Qt::PreferredSize).height());
    }

    return QSizeF(width, height);
}

void MSliderIndicator::setLabelFixedWidth(const qreal width)
{
    fixedWidth = width;
    updateGeometry();
}

MSliderHandleIndicator::MSliderHandleIndicator(QGraphicsItem* parent) :
    MWidget(parent),
    arrowPos(0),
    arrowMargin(0),
    orientation(Qt::Horizontal),
    indicatorArrow(0),
    indicator(0)
{
    indicator = new MSliderIndicator(false, this);
    indicator->setObjectName("MSliderHandleIndicator");

    indicatorArrow = new MSliderHandleIndicatorArrow(this);
    indicatorArrow->setObjectName("MSliderHandleIndicatorArrow");
}

MSliderHandleIndicator::~MSliderHandleIndicator()
{
}

void MSliderHandleIndicator::setOrientation(Qt::Orientation orientation)
{
    this->orientation = orientation;
    indicatorArrow->setOrientation(orientation);

    bool reverse = qApp->isRightToLeft();

    if (orientation == Qt::Horizontal) {
        indicator->setPos(0, 0);
        qreal arrowX = qMax(qreal(0), arrowPos - indicatorArrow->rect().width() / 2);
        indicatorArrow->setPos(arrowX, indicator->rect().bottom());
    }

    if (orientation == Qt::Vertical) {
        qreal arrowY = qMax(qreal(0), arrowPos - indicatorArrow->rect().height() / 2);
        if (!reverse) {
            indicatorArrow->setPos(0, arrowY);
            indicator->setPos(indicatorArrow->rect().right(), 0);
        } else {
            indicator->setPos(0, 0);
            indicatorArrow->setPos(indicator->rect().right(), arrowY);
        }
    }
    updateGeometry();
}

void MSliderHandleIndicator::setPixmaps(const QPixmap *handleLabelArrowLeft,
                                        const QPixmap *handleLabelArrowRight,
                                        const QPixmap *handleLabelArrowUp,
                                        const QPixmap *handleLabelArrowDown)
{
    indicatorArrow->setPixmaps(handleLabelArrowLeft,
                               handleLabelArrowRight,
                               handleLabelArrowUp,
                               handleLabelArrowDown);

    updateGeometry();
}

void MSliderHandleIndicator::setStyleName(const QString &stylename)
{
    indicator->setLabelStyleName(stylename);
}

void MSliderHandleIndicator::setText(const QString &text)
{
    indicator->setText(text);
    indicatorArrow->setVisible(!indicator->isEmpty());

    updateGeometry();
}

void MSliderHandleIndicator::setImage(const QString &id)
{
    indicator->setImage(id);
    indicatorArrow->setVisible(!indicator->isEmpty());

    updateGeometry();
}

void MSliderHandleIndicator::setArrowPos(qreal pos)
{
    arrowPos = pos;
    bool reverse = qApp->isRightToLeft();

    if (orientation == Qt::Horizontal) {
        qreal rightLimit = indicator->rect().width() - indicatorArrow->rect().width() - arrowMargin;
        qreal arrowX;
        if (arrowMargin <= rightLimit)
            arrowX = qBound(arrowMargin, arrowPos - indicatorArrow->rect().width() / 2, rightLimit);
        else
            arrowX = indicator->rect().width() / 2 - indicatorArrow->rect().width() / 2;

        indicatorArrow->setPos(arrowX, indicator->rect().bottom());
    }

    if (orientation == Qt::Vertical) {
        qreal bottomLimit = indicator->rect().height() - indicatorArrow->rect().height() - arrowMargin;
        qreal arrowY;
        if (arrowMargin <= bottomLimit)
            arrowY = qBound(arrowMargin, arrowPos - indicatorArrow->rect().height() / 2, bottomLimit);
        else
            arrowY = indicator->rect().height() / 2 - indicatorArrow->rect().height() / 2;

        if (!reverse)
            indicatorArrow->setPos(0, arrowY);
        else
            indicatorArrow->setPos(indicator->rect().right(), arrowY);
    }
}

void MSliderHandleIndicator::setArrowMargin(qreal margin)
{
    arrowMargin = margin;

    updateGeometry();
}

void MSliderHandleIndicator::moveOnTopAllSiblings()
{
    QGraphicsItem* parent = parentItem();

    if (!parent)
        return;

    QList<QGraphicsItem *> childItems = parent->childItems();

    if (!childItems.empty()) {
        int zValue = childItems.first()->zValue();
        for (int i = 1; i < childItems.size(); ++i) {
            if (zValue < childItems.at(i)->zValue())
                zValue = childItems.at(i)->zValue();
        }

        setZValue(zValue + 1);
    }
}

QSizeF MSliderHandleIndicator::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    QSizeF size = indicator->size();

    if (orientation == Qt::Horizontal) {
        size.setWidth(qMax(size.width(), indicatorArrow->size().width()));
        size.setHeight(size.height() + indicatorArrow->size().height());
    }

    if (orientation == Qt::Vertical) {
        size.setWidth(size.width() + indicatorArrow->size().width());
        size.setHeight(qMax(size.height(), indicatorArrow->size().height()));
    }

    return size;
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
    grooveMargin(0),
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
    maximumLength(0),
    indicatorMargin(0)
{
    sliderHandle = new MSliderHandle(this);
    sliderHandle->setObjectName("MSliderHandle");

    sliderHandleIndicator = new MSliderHandleIndicator(0);
    sliderHandleIndicator->setObjectName("MSliderHandleLabel");
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
    sliderHandleIndicator->setOrientation(orientation);

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

void MSliderGroove::setHandleIndicatorPixmaps(const QPixmap *handleLabelArrowLeft,
                                                const QPixmap *handleLabelArrowRight,
                                                const QPixmap *handleLabelArrowUp,
                                                const QPixmap *handleLabelArrowDown)
{
    sliderHandleIndicator->setPixmaps(handleLabelArrowLeft,
                                      handleLabelArrowRight,
                                      handleLabelArrowUp,
                                      handleLabelArrowDown);

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

void MSliderGroove::setGrooveMargin(qreal margin)
{
    grooveMargin = margin;

    updateGeometry();
}

void MSliderGroove::setIndicatorMargin(qreal margin)
{
    indicatorMargin = margin;

    updateGeometry();
}

void MSliderGroove::setHandleIndicatorArrowMargin(qreal margin)
{
    sliderHandleIndicator->setArrowMargin(margin);

    updateGeometry();
}

void MSliderGroove::setElapsedOffset(qreal offset)
{
    elapsedOffset = offset;

    updateGeometry();
}

void MSliderGroove::setIndicatorOffset(qreal offset)
{
    indicatorOffset = offset;

    updateGeometry();
}

void MSliderGroove::setIndicatorStyleName(const QString &stylename)
{
    sliderHandleIndicator->setStyleName(stylename);
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

    updateHandlePos();

    if (controller)
        controller->update();
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

    if (controller)
        controller->update();
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
    QPointF handlePoint = point - pos();
    qreal coordinate = 0.0f;
    int offset = 0;
    QRectF valueRangeRect = rect();
    int range = maximum - minimum;
    qreal w = sliderHandle->rect().width();

    if (minimum == maximum)
        return minimum;

    if (orientation == Qt::Horizontal) {
        valueRangeRect.adjust(w/2.0, 0, -w/2.0, 0);
        coordinate = handlePoint.x();
    } else if (orientation == Qt::Vertical) {
        valueRangeRect.adjust(0, grooveMargin, 0, -grooveMargin);
        coordinate = handlePoint.y();
    }

    if (orientation == Qt::Horizontal) {
        if (!qApp->isRightToLeft())
            offset = qRound(((coordinate - valueRangeRect.left()) * range) / valueRangeRect.width());
        else
            offset = qRound(((valueRangeRect.right() - coordinate) * range) / valueRangeRect.width());
    } else if (orientation == Qt::Vertical) {
        offset = qRound(((valueRangeRect.bottom() - coordinate) * range) / valueRangeRect.height());
    }

    return qBound(minimum, minimum + offset, maximum);
}

//determines clickable area which is basically the
//exact area of slider groove
QRectF MSliderGroove::clickableArea() const
{
    return rect();
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
        //reparents slider handle indicator only if it is necessary
        if (newParent != sliderHandleIndicator->parentItem()) {
            sliderHandleIndicator->setParentItem(newParent);
            sliderHandleIndicator->moveOnTopAllSiblings();
        }

        //by raising handle indicator will be shown (only if it
        //is not already visible)
        if (!sliderHandleIndicator->isVisible())
            sliderHandleIndicator->setVisible(true);
    }

    updateHandleIndicatorPos();
}

void MSliderGroove::lowerHandleIndicator()
{
    //by lowering  handle indicator will be also hidden
    sliderHandleIndicator->setVisible(false);

    //This effectively removes the sliderHandleIndicator item from the scene.
    sliderHandleIndicator->setParentItem(0);

    updateHandleIndicatorPos();
}

void MSliderGroove::ensureSafeClosing()
{
    delete sliderHandleIndicator;
    sliderHandleIndicator = 0;
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
        qreal handle_center = sliderHandle->pos().x() + sliderHandle->rect().width() / 2.0;

        if (backgroundBaseImage) {
            backgroundBaseImage->borders(&left, &right, &top, &bottom);
            horizontalMargins = left + right;
        }

        qreal vAdjustment = (grooveRect.height() - grooveThickness) / 2;
        grooveRect.adjust(grooveMargin, vAdjustment, -grooveMargin, -vAdjustment);

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
                    elapsedRect.setRight(qMin(handle_center + elapsedOffset,
                                              grooveRect.right()));
            }
            else {
                if (value < maximum)
                    elapsedRect.setLeft(qMax(handle_center - elapsedOffset,
                                             grooveRect.left()));
            }

            if (elapsedRect.width() >= qreal(horizontalMargins))
                backgroundElapsedImage->draw(elapsedRect.toRect(), painter);
        }
    }

    if (orientation == Qt::Vertical) {
        qreal handle_center = sliderHandle->pos().y() + sliderHandle->rect().height() / 2.0;

        if (backgroundVerticalBaseImage) {
            backgroundVerticalBaseImage->borders(&left, &right, &top, &bottom);
            verticalMargins = top + bottom;
        }

        qreal hAdjustment = (grooveRect.width() - grooveThickness) / 2;
        grooveRect.adjust(hAdjustment, grooveMargin, -hAdjustment, -grooveMargin);

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
                elapsedRect.setTop(qMin(handle_center + elapsedOffset,
                                        grooveRect.top()));
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

    updateHandlePos();
}

QSizeF MSliderGroove::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);

    qreal preferredWidth;
    qreal preferredHeight;

    switch (which) {
    case Qt::MinimumSize: {
        if (orientation == Qt::Horizontal) {
            preferredHeight = qMax(sliderHandle->sizeHint(Qt::PreferredSize).height(), grooveThickness);
            return QSizeF(minimumLength, preferredHeight);
        }
        if (orientation == Qt::Vertical) {
            preferredWidth = qMax(sliderHandle->sizeHint(Qt::PreferredSize).width(), grooveThickness);
            return QSizeF(preferredWidth, minimumLength);
        }
        break;
    }
    case Qt::PreferredSize: {
        if (orientation == Qt::Horizontal) {
            preferredHeight = qMax(sliderHandle->sizeHint(Qt::PreferredSize).height(), grooveThickness);
            return QSizeF(preferredLength, preferredHeight);
        }
        if (orientation == Qt::Vertical) {
            preferredWidth = qMax(sliderHandle->sizeHint(Qt::PreferredSize).width(), grooveThickness);
            return QSizeF(preferredWidth, preferredLength);
        }
        break;
    }
    case Qt::MaximumSize: {
        if (orientation == Qt::Horizontal) {
            preferredHeight = qMax(sliderHandle->sizeHint(Qt::PreferredSize).height(), grooveThickness);
            return QSizeF(maximumLength, preferredHeight);
        }
        if (orientation == Qt::Vertical) {
            preferredWidth = qMax(sliderHandle->sizeHint(Qt::PreferredSize).width(), grooveThickness);
            return QSizeF(preferredWidth, maximumLength);
        }
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
void MSliderGroove::updateHandlePos()
{
    qreal w = sliderHandle->rect().width();
    qreal h = sliderHandle->rect().height();
    QPointF newPos((rect().width() - w) / 2.0f, (rect().height() - h) / 2.0f);

    int range = maximum - minimum;
    QRectF valueRangeRect = rect();

    if (minimum != maximum) {
        if (orientation == Qt::Horizontal) {
            valueRangeRect.adjust(0, 0, - w, 0);

            if (!qApp->isRightToLeft())
                newPos.setX(valueRangeRect.left() + (value - minimum) * valueRangeRect.width() / range);
            else
                newPos.setX(valueRangeRect.left() + (maximum - value) * valueRangeRect.width() / range);
        } else if (orientation == Qt::Vertical) {
            valueRangeRect.adjust(0, h/2.0, 0, - w / 2.0);

            newPos.setX(valueRangeRect.top() + (maximum - value) * valueRangeRect.height() / range);
        }
    }

    if (newPos != sliderHandle->pos()) {
        sliderHandle->setPos(newPos);
        updateHandleIndicatorPos();
    }
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
        handleIndicatorPos.setY(handleIndicatorPos.y() - indicatorOffset);
    }
    if (orientation == Qt::Vertical) {
        if (!reverse)
            handleIndicatorPos.setX(handleIndicatorPos.x() + indicatorOffset);
        else
            handleIndicatorPos.setX(handleIndicatorPos.x() - indicatorOffset);

        handleIndicatorPos.setY(handleIndicatorPos.y() +
                                (sliderHandle->rect().height() / 2) - (sliderHandleIndicator->rect().height() / 2));
    }

    QPointF groovePos = pos();
    sliderBoundingRect = controller->rect();
    sliderBoundingRect.translate(-groovePos);

    if (orientation == Qt::Horizontal) {
        qreal leftLimit = qMax(sliderBoundingRect.left(), indicatorMargin);
        qreal rightLimit = qMin(sliderBoundingRect.right(), rect().width() - sliderHandleIndicator->rect().width() - indicatorMargin);
        if (leftLimit <= rightLimit)
            handleIndicatorPos.setX(qBound(leftLimit, handleIndicatorPos.x(), rightLimit));
        else
            handleIndicatorPos.setX(qMax(sliderBoundingRect.left(),
                                         rect().width() / 2 - sliderHandleIndicator->rect().width() / 2));
        sliderHandleIndicator->setArrowPos(sliderHandle->pos().x() + sliderHandle->rect().width() / 2 - handleIndicatorPos.x());
    }
    if (orientation == Qt::Vertical) {
        qreal topLimit = qMax(sliderBoundingRect.top(), indicatorMargin);
        qreal bottomLimit = qMin(sliderBoundingRect.bottom(), rect().height() - sliderHandleIndicator->rect().height() - indicatorMargin);
        if (topLimit <= bottomLimit)
            handleIndicatorPos.setY(qBound(topLimit, handleIndicatorPos.y(), bottomLimit));
        else
            handleIndicatorPos.setY(qMax(sliderBoundingRect.top(),
                                         rect().height() / 2 - sliderHandleIndicator->rect().height() / 2));
        sliderHandleIndicator->setArrowPos(sliderHandle->pos().y() + sliderHandle->rect().height() / 2 - handleIndicatorPos.y());
    }

    QGraphicsItem *sliderHandleIndicatorParent = sliderHandleIndicator->parentItem();
    QPointF grooveRelativePos;
    if (sliderHandleIndicatorParent)
        grooveRelativePos = mapFromScene(scenePos()) - mapFromScene(sliderHandleIndicatorParent->scenePos());

    //slider handle label position (and bounding rect)
    //will be changed only if it is really necessary
    QPointF newPos = grooveRelativePos + handleIndicatorPos;
    if (newPos == pos()) {
        return;
    }

    sliderHandleIndicator->setPos(newPos);

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
            valueRangeRect.adjust(grooveMargin, 0, -grooveMargin, 0);
            beginning = valueRangeRect.left();

            if (!reverse)
                offset = (value - minimum) * valueRangeRect.width() / range;
            else
                offset = (maximum - value) * valueRangeRect.width() / range;
        } else if (orientation == Qt::Vertical) {
            valueRangeRect.adjust(0, grooveMargin, 0, -grooveMargin);
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
    positionAnimation(0),
    previousValue(0),
    feedbackPlayedFor(0),
    pressTimerId(0),
    valueWhenPressed(0),
    position(0)
{
    timeOnFeedback.invalidate();
    timeOnMove.invalidate();
}

MSliderViewPrivate::~MSliderViewPrivate()
{
    sliderGroove->ensureSafeClosing();

    delete positionAnimation;
}

//intializes main layout and layout policies
void MSliderViewPrivate::init(MSlider *controller)
{
    this->controller = controller;

    sliderGroove = new MSliderGroove(controller);
    sliderGroove->setObjectName("MSliderGroove");

    sliderGroove->init(controller);

    sliderGroove->lowerHandleIndicator();

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

QPropertyAnimation* MSliderViewPrivate::createPositionAnimation()
{
    Q_Q(MSliderView);

    QPropertyAnimation *animation = positionAnimation = new QPropertyAnimation(q, "position", 0);
    animation->setDuration(150);
    animation->setEasingCurve(QEasingCurve::OutSine);

    return animation;
}

//sets slider value to that one corresponding
//to mouse cursor position
int MSliderViewPrivate::updateValue(QGraphicsSceneMouseEvent *event)
{
    Q_Q(MSliderView);

    bool needAnimation = false;
    int newValue;

    QRectF clickableHandleRect = sliderGroove->clickableHandleArea();
    clickableHandleRect.translate(sliderGroove->pos());

    //animation is necessary when tap point is further form
    //slider handle middle point then slider handle width / height
    //(depending on slider orientation)
    if (q->model()->orientation() == Qt::Horizontal) {
        if (qAbs(event->pos().x() - clickableHandleRect.center().x()) > clickableHandleRect.width())
            needAnimation = true;
    } else {
        if (qAbs(event->pos().y() - clickableHandleRect.center().y()) > clickableHandleRect.height())
            needAnimation = true;
    }

    //there are some margins around the view
    //and those have to be considered
    QPointF eventPos = event->pos();
    eventPos.setX(eventPos.x() + q->marginLeft());
    eventPos.setY(eventPos.y() + q->marginTop());

    newValue = adjustValue(q->model()->minimum(),
                           sliderGroove->screenPointToValue(eventPos),
                           q->model()->maximum(),
                           q->model()->steps());
    //sometimes this method can be called twice with the same
    //event position (for example when user clicks to slider groove
    //once it is called at mouse press and once at mouse release)
    if (newValue != q->model()->value()) {
        if (needAnimation) {
            if (!positionAnimation) {
                positionAnimation = createPositionAnimation();
            }
            positionAnimation->setEndValue(newValue);
            positionAnimation->start();
        } else {
            position = newValue;
        }
        controller->setValue(newValue);
    }

    return newValue;
}

// adjusts the slider value according to the defined steps
int MSliderViewPrivate::adjustValue(int minimum, int value, int maximum, int steps)
{
    if (steps < 0)
        steps = -steps;

    if ((steps < 1) || (steps > maximum - minimum - 1))
        return value;

    //minimum or maximum are always valid values regardless
    //to the number of steps, values outside of slider range will
    //not be adjusted because model forces them to minimum
    //of maximum values
    if ((value <= minimum) || (value >= maximum))
        return value;

    int numberOfStep = qRound(((qreal(value) - qreal(minimum)) * qreal(steps)) / (qreal(maximum) - qreal(minimum)));
    qreal discreteValue = minimum + (qreal(numberOfStep) * (qreal(maximum) - qreal(minimum)) / qreal(steps));
    return qRound(discreteValue);
}

//refreshes slider groove (min, max and value, slider state)
void MSliderViewPrivate::updateSliderGroove()
{
    Q_Q(MSliderView);

    sliderGroove->setSliderValues(q->model()->minimum(), q->model()->maximum(), position);
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

void MSliderViewPrivate::updateMoveFeedbackData(int newValue)
{
    Q_Q(MSliderView);

    // Find a number closest to slider value which is divisible by
    // steps-per-feedback. This is the slider value which will trigger
    // a feedback when crossed.
    feedbackPlayedFor = q->style()->stepsPerFeedback() * qRound(static_cast<qreal>(newValue) / q->style()->stepsPerFeedback());
}

void MSliderViewPrivate::playSliderMoveFeedback(int newValue, const QPointF& newPosition)
{
    Q_Q(MSliderView);

    if (newValue != previousValue) {
        int moveLimit, moveAmount;

        moveLimit = MDeviceProfile::instance()->mmToPixels(q->style()->feedbackSpeedLimit()) * timeOnMove.restart();
        if (q->model()->orientation() == Qt::Horizontal) {
            moveAmount = qAbs(previousPosition.x() - newPosition.x()) * 1000;
        } else {
            moveAmount = qAbs(previousPosition.y() - newPosition.y()) * 1000;
        }

        // Consider playing feedback if slider is moving slow enough
        if (moveAmount < moveLimit) {
            // Play feedback if:
            //  - Enough time has elapsed since previous playing of feedback
            //  - Slider value crosses a value that is divisible by steps-per-feedback
            if (timeOnFeedback.isValid() &&
                timeOnFeedback.elapsed() > q->style()->minimumFeedbackInterval() &&
                (qAbs(feedbackPlayedFor - newValue) >= q->style()->stepsPerFeedback() ||
                (newValue <= feedbackPlayedFor && previousValue > feedbackPlayedFor) ||
                (newValue >= feedbackPlayedFor && previousValue < feedbackPlayedFor))) {
                q->style()->moveFeedback().play();
                timeOnFeedback.start();
                updateMoveFeedbackData(newValue);
            }
        } else {
            updateMoveFeedbackData(newValue);
        }

        previousValue = newValue;
    }

    previousPosition = newPosition;
}

MSliderView::MSliderView(MSlider *controller):
    MWidgetView(controller),
    d_ptr(new MSliderViewPrivate)
{
    Q_D(MSliderView);
    d->q_ptr = this;
    d->init(controller);

    controller->grabGesture(Qt::SwipeGesture);

    connect(controller, SIGNAL(visibleChanged()), this, SLOT(changeSliderHandleIndicatorVisibility()));
    connect(controller, SIGNAL(displayExited()), this, SLOT(lowerSliderHandleIndicator()));
}

MSliderView::~MSliderView()
{
    delete d_ptr;
}

void MSliderView::setGeometry(const QRectF &rect)
{
    MWidgetView::setGeometry(rect);

    Q_D(MSliderView);
    bool bigEnough  = rect.contains(d->controller->layout()->geometry());
    d->controller->setFlag(QGraphicsItem::ItemClipsChildrenToShape, !bigEnough);
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
        else if (member == MSliderModel::Value) {
            // The position should not be set here if the position animation
            // is running or else it would not let the animation play correctly
            if (!(d->controller->isVisible() && d->controller->isOnDisplay()) ||
                !d->positionAnimation ||
                (d->positionAnimation->state() != QAbstractAnimation::Running)) {
                setPosition(model()->value());
                continue;
            }
        }
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

    setPosition(model()->value());
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
    d->sliderGroove->setHandleIndicatorPixmaps(style()->handleLabelArrowLeftPixmap(),
                                               style()->handleLabelArrowRightPixmap(),
                                               style()->handleLabelArrowUpPixmap(),
                                               style()->handleLabelArrowDownPixmap());
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

    d->sliderGroove->setIndicatorMargin(style()->handleLabelMargin());
    d->sliderGroove->setHandleIndicatorArrowMargin(style()->handleLabelArrowMargin());
    d->sliderGroove->setElapsedOffset(style()->elapsedOffset());
    d->sliderGroove->setIndicatorOffset(style()->indicatorOffset());
    d->sliderGroove->setIndicatorStyleName(style()->indicatorStyleName());

    d->sliderGroove->setGrooveMargin(style()->grooveMargin());

    d->minIndicator->setLabelFixedWidth(style()->minLabelFixedWidth());
    d->maxIndicator->setLabelFixedWidth(style()->maxLabelFixedWidth());

    d->updateOrientation();

    //only to reposition slider handle
    d->updateSliderGroove();

    updateGeometry();
}

void MSliderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MSliderView);

    if (d->controller->isVisible() && d->controller->isOnDisplay()) {
        if (d->isCollision(event)) {
            d->valueWhenPressed = model()->value();
            d->controller->setState(MSliderModel::Pressed);
            style()->pressFeedback().play();
            d->timeOnFeedback.start();
            d->timeOnMove.start();
            d->previousPosition = event->pos();
            d->previousValue = d->updateValue(event);
            d->updateMoveFeedbackData(d->previousValue);
            d->pressTimerId = startTimer(100);
        }
    }
}

void MSliderView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MSliderView);

    d->controller->setState(MSliderModel::Released);

    style()->cancelFeedback().play();

    if (d->pressTimerId) {
        killTimer(d->pressTimerId);
        d->pressTimerId = 0;
    }

    if (d->positionAnimation)
        d->positionAnimation->stop();

    model()->setValue(d->valueWhenPressed);
    d->sliderGroove->lowerHandleIndicator();
}

void MSliderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MSliderView);

    if (d->controller->isVisible() && d->controller->isOnDisplay()) {
        style()->releaseFeedback().play();

        if (d->isCollision(event))
            d->updateValue(event);
    }

    if (d->pressTimerId) {
        killTimer(d->pressTimerId);
        d->pressTimerId = 0;
    }

    d->controller->setState(MSliderModel::Released);

    d->sliderGroove->lowerHandleIndicator();
}

void MSliderView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MSliderView);

    if (d->controller->isVisible() && d->controller->isOnDisplay()) {
        if (d->controller->state() == MSliderModel::Pressed) {
            int newValue = d->updateValue(event);
            d->playSliderMoveFeedback(newValue, event->pos());

            if (model()->handleLabelVisible())
                d->sliderGroove->raiseHandleIndicator();
        } else
            lowerSliderHandleIndicator();
    } else {
        if (d->controller->state() == MSliderModel::Pressed) {
            d->controller->setState(MSliderModel::Released);
 
            style()->cancelFeedback().play();
 
            if (d->pressTimerId) {
                killTimer(d->pressTimerId);
                d->pressTimerId = 0;
            }
 
            if (d->positionAnimation)
                d->positionAnimation->stop();
 
            d->sliderGroove->lowerHandleIndicator();
        }
    }
}

void MSliderView::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
    event->accept(gesture);
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

void MSliderView::changeSliderHandleIndicatorVisibility()
{
    Q_D(MSliderView);

    if (!d->controller->isVisible())
        lowerSliderHandleIndicator();
}

int MSliderView::position() const
{
    Q_D(const MSliderView);

    return d->position;
}

void MSliderView::setPosition(int position)
{
    Q_D(MSliderView);

    d->position = position;
    d->updateSliderGroove();
}

M_REGISTER_VIEW_NEW(MSliderView, MSlider)
