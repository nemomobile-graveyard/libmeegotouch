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

#ifndef MSLIDERVIEW_P_H
#define MSLIDERVIEW_P_H

#include "msliderview.h"

#include <MWidget>
#include "mslidermodel.h"

class MSlider;
class QPixmap;
class MScalableImage;
class MLabel;
class MImageWidget;
class QGraphicsAnchorLayout;
class MLinearLayoutPolicy;
class MSliderView;
class QPropertyAnimation;

class MSliderHandle : public MWidget
{
public:
    MSliderHandle(QGraphicsItem *parent = 0);
    virtual ~MSliderHandle();

    void setOrientation(Qt::Orientation);
    void setPixmaps(const QPixmap *handle,
                    const QPixmap *handlePressed,
                    const QPixmap *handleVertical,
                    const QPixmap *handleVerticalPressed);

    void setSliderState(MSliderModel::SliderState state);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private:
    Qt::Orientation orientation;

    const QPixmap *handlePixmap;
    const QPixmap *handlePressedPixmap;
    const QPixmap *handleVerticalPixmap;
    const QPixmap *handleVerticalPressedPixmap;

    MSliderModel::SliderState sliderState;

#ifdef UNIT_TEST
    friend class Ut_MSliderView;
#endif // UNIT_TEST
};

class MSliderIndicator : public MWidget
{
    Q_OBJECT
public:
    MSliderIndicator(bool isMinMax, QGraphicsItem *parent = 0);
    virtual ~MSliderIndicator();

    void setText(const QString &text);
    void setImage(const QString &id);

protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private:
    MLabel *label;
    MImageWidget *image;
    QString imageName;
    //bool visibility;

    QGraphicsAnchorLayout *layout;

#ifdef UNIT_TEST
    friend class Ut_MSliderView;
#endif // UNIT_TEST
};

class MSliderGroove : public MWidget
{
    Q_OBJECT
public:
    MSliderGroove(QGraphicsItem *parent = 0);
    virtual ~MSliderGroove();

    void init(MSlider *controller);

    void setOrientation(Qt::Orientation);

    void setHandlePixmaps(const QPixmap *handle,
                          const QPixmap *handlePressed,
                          const QPixmap *handleVertical,
                          const QPixmap *handleVerticalPressed);
    void setImages(const MScalableImage *background,
                   const MScalableImage *backgroundElapsed,
                   const MScalableImage *backgroundReceived,
                   const MScalableImage *backgroundVertical,
                   const MScalableImage *backgroundVerticalElapsed,
                   const MScalableImage *backgroundVerticalReceived);
    void setGrooveThickness(qreal thickness);
    void setGrooveLength(qreal prefLength, qreal minLength, qreal maxLength);

    void setSliderValues(int min, int max, int val);
    void setSeekBarValues(bool show, int loadedContentMin = 0, int loadedContentMax = 0);
    void setSliderState(MSliderModel::SliderState state);

    void setIndicatorText(const QString &text);
    void setIndicatorImage(const QString &id);

    int screenPointToValue(const QPointF &point) const;

    QRectF clickableArea() const;
    QRectF clickableHandleArea() const;

    QRectF boundingRect() const;

    void raiseHandleIndicator();
    void lowerHandleIndicator();

    void ensureSafeClosing();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

    virtual void setGeometry(const QRectF &rect);
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    void updateHandlePos(const QPointF &position);
    void updateHandleIndicatorPos();

    MSlider *controller;

    Qt::Orientation orientation;

    qreal valueToScreenCoordinate(int value) const;

    const MScalableImage *backgroundBaseImage;
    const MScalableImage *backgroundElapsedImage;
    const MScalableImage *backgroundReceivedImage;
    const MScalableImage *backgroundVerticalBaseImage;
    const MScalableImage *backgroundVerticalElapsedImage;
    const MScalableImage *backgroundVerticalReceivedImage;

    qreal grooveThickness;

    int minimum;
    int maximum;
    int value;

    int loadedContentMinimum;
    int loadedContentMaximum;
    bool showSeekBar;


    MSliderHandle *sliderHandle;
    MSliderIndicator *sliderHandleIndicator;

    QRectF sliderBoundingRect;

    qreal preferredLength;
    qreal minimumLength;
    qreal maximumLength;

#ifdef UNIT_TEST
    friend class Ut_MSliderView;
#endif // UNIT_TEST
};

class MSliderViewPrivate
{
    Q_DECLARE_PUBLIC(MSliderView)
    
protected:
    MSliderView *q_ptr;
    
public:
    MSliderViewPrivate();
    virtual ~MSliderViewPrivate();

    MSlider *controller;

    MSliderGroove *sliderGroove;

    MSliderIndicator *minIndicator;
    MSliderIndicator *maxIndicator;

    MLinearLayoutPolicy *horizontalPolicy;
    MLinearLayoutPolicy *verticalPolicy;

    QPropertyAnimation *valueAnimation;

    int pressTimerId;

    int valueWhenPressed;

    void init(MSlider *contoller);

    void insertMinIndicatorToLayout();
    void insertMaxIndicatorToLayout();
    void removeMinIndicatorFromLayout();
    void removeMaxIndicatorFromLayout();

    void updateOrientation();
    bool isCollision(QGraphicsSceneMouseEvent *event) const;
    void updateValue(QGraphicsSceneMouseEvent *event);
    void updateSliderGroove();
    void updateSeekBar();

#ifdef UNIT_TEST
    friend class Ut_MSliderView;
#endif // UNIT_TEST
};

#endif
