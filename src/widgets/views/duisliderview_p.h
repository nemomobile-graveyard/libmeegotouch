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

#ifndef DUISLIDERVIEW_P_H
#define DUISLIDERVIEW_P_H

#include "duisliderview.h"
#include "private/duiwidgetview_p.h"

#include <DuiWidget>
#include "duislidermodel.h"

class DuiSlider;
class QPixmap;
class DuiScalableImage;
class DuiLabel;
class DuiImageWidget;
class QGraphicsAnchorLayout;
class DuiLinearLayoutPolicy;
class DuiSliderView;
class QPropertyAnimation;

class DuiSliderHandle : public DuiWidget
{
public:
    DuiSliderHandle(QGraphicsItem *parent = 0);
    virtual ~DuiSliderHandle();

    void setOrientation(Qt::Orientation);
    void setPixmaps(const QPixmap *handle,
                    const QPixmap *handlePressed,
                    const QPixmap *handleVertical,
                    const QPixmap *handleVerticalPressed);

    void setSliderState(DuiSliderModel::SliderState state);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private:
    Qt::Orientation orientation;

    const QPixmap *handlePixmap;
    const QPixmap *handlePressedPixmap;
    const QPixmap *handleVerticalPixmap;
    const QPixmap *handleVerticalPressedPixmap;

    DuiSliderModel::SliderState sliderState;

#ifdef UNIT_TEST
    friend class Ut_DuiSliderView;
#endif // UNIT_TEST
};

class DuiSliderIndicator : public DuiWidget
{
    Q_OBJECT
public:
    DuiSliderIndicator(bool isMinMax, QGraphicsItem *parent = 0);
    virtual ~DuiSliderIndicator();

    void setText(const QString &text);
    void setImage(const QString &id);
    void setVisible(bool visible);

protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private:
    DuiLabel *label;
    DuiImageWidget *image;
    QString imageName;

    QGraphicsAnchorLayout *layout;

#ifdef UNIT_TEST
    friend class Ut_DuiSliderView;
#endif // UNIT_TEST
};

class DuiSliderGroove : public DuiWidget
{
    Q_OBJECT
public:
    DuiSliderGroove(QGraphicsItem *parent = 0);
    virtual ~DuiSliderGroove();

    void init(DuiSlider *controller);

    void setOrientation(Qt::Orientation);

    void setHandlePixmaps(const QPixmap *handle,
                          const QPixmap *handlePressed,
                          const QPixmap *handleVertical,
                          const QPixmap *handleVerticalPressed);
    void setImages(const DuiScalableImage *background,
                   const DuiScalableImage *backgroundElapsed,
                   const DuiScalableImage *backgroundReceived,
                   const DuiScalableImage *backgroundVertical,
                   const DuiScalableImage *backgroundVerticalElapsed,
                   const DuiScalableImage *backgroundVerticalReceived);
    void setGrooveThickness(qreal thickness);
    void setGrooveLength(qreal prefLength, qreal minLength, qreal maxLength);

    void setSliderValues(int min, int max, int val);
    void setSeekBarValues(bool show, int loadedContentMin = 0, int loadedContentMax = 0);
    void setSliderState(DuiSliderModel::SliderState state);

    void setIndicatorText(const QString &text);
    void setIndicatorImage(const QString &id);
    void setIndicatorVisible(bool visible);

    int screenPointToValue(const QPointF &point) const;

    QRectF clickableArea() const;
    QRectF clickableHandleArea() const;

    QRectF boundingRect() const;

    void raiseHandleIndicator();
    void lowerHandleIndicator();

    void ensureSafeClosing();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

    void setGeometry(const QRectF &rect);
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    void updateHandlePos(const QPointF &position);
    void updateHandleIndicatorPos();

    DuiSlider *controller;

    Qt::Orientation orientation;

    qreal valueToScreenCoordinate(int value) const;

    const DuiScalableImage *backgroundBaseImage;
    const DuiScalableImage *backgroundElapsedImage;
    const DuiScalableImage *backgroundReceivedImage;
    const DuiScalableImage *backgroundVerticalBaseImage;
    const DuiScalableImage *backgroundVerticalElapsedImage;
    const DuiScalableImage *backgroundVerticalReceivedImage;

    qreal grooveThickness;

    int minimum;
    int maximum;
    int value;

    int loadedContentMinimum;
    int loadedContentMaximum;
    bool showSeekBar;


    DuiSliderHandle *sliderHandle;
    DuiSliderIndicator *sliderHandleIndicator;

    QRectF sliderBoundingRect;

    qreal preferredLength;
    qreal minimumLength;
    qreal maximumLength;

#ifdef UNIT_TEST
    friend class Ut_DuiSliderView;
#endif // UNIT_TEST
};

class DuiSliderViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiSliderView)

public:
    DuiSliderViewPrivate();
    virtual ~DuiSliderViewPrivate();

    DuiSlider *controller;

    DuiSliderGroove *sliderGroove;

    DuiSliderIndicator *minIndicator;
    DuiSliderIndicator *maxIndicator;

    DuiLinearLayoutPolicy *horizontalPolicy;
    DuiLinearLayoutPolicy *verticalPolicy;

    QPropertyAnimation *valueAnimation;

    int pressTimerId;

    int valueWhenPressed;

    void init(DuiSlider *contoller);

    void updateOrientation();
    bool isCollision(QGraphicsSceneMouseEvent *event) const;
    void updateValue(QGraphicsSceneMouseEvent *event);
    void updateSliderGroove();
    void updateSeekBar();

#ifdef UNIT_TEST
    friend class Ut_DuiSliderView;
#endif // UNIT_TEST
};

#endif
