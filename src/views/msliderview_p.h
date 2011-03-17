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
#include <QElapsedTimer>
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

class MSliderHandleIndicatorArrow : public MWidget
{
public:
    MSliderHandleIndicatorArrow(QGraphicsItem *parent = 0);
    virtual ~MSliderHandleIndicatorArrow();

    void setOrientation(Qt::Orientation);
    void setPixmaps(const QPixmap *handleLabelArrowLeft,
                    const QPixmap *handleLabelArrowRight,
                    const QPixmap *handleLabelArrowUp,
                    const QPixmap *handleLabelArrowDown);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private:
    Qt::Orientation orientation;

    const QPixmap *handleLabelArrowLeftPixmap;
    const QPixmap *handleLabelArrowRightPixmap;
    const QPixmap *handleLabelArrowUpPixmap;
    const QPixmap *handleLabelArrowDownPixmap;

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

    void setLabelStyleName(const QString &stylename);
    void setText(const QString &text);
    void setImage(const QString &id);
    void setLabelFixedWidth(const qreal width);

    bool isEmpty() const;

protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private:
    MLabel *label;
    MImageWidget *image;
    QString imageName;
    qreal fixedWidth;

    QGraphicsAnchorLayout *layout;

#ifdef UNIT_TEST
    friend class Ut_MSliderView;
#endif // UNIT_TEST
};

class MSliderHandleIndicator : public MWidget
{
    Q_OBJECT
public:
    MSliderHandleIndicator(QGraphicsItem *parent = 0);
    virtual ~MSliderHandleIndicator();

    void setOrientation(Qt::Orientation);
    void setPixmaps(const QPixmap *handleLabelArrowLeft,
                    const QPixmap *handleLabelArrowRight,
                    const QPixmap *handleLabelArrowUp,
                    const QPixmap *handleLabelArrowDown);

    void setStyleName(const QString &stylename);
    void setText(const QString &text);
    void setImage(const QString &id);
    void setArrowPos(qreal pos);
    void setArrowMargin(qreal margin);

    void moveOnTopAllSiblings();

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private:
    qreal arrowPos;
    qreal arrowMargin;
    Qt::Orientation orientation;
    MSliderHandleIndicatorArrow* indicatorArrow;
    MSliderIndicator* indicator;
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
    void setHandleIndicatorPixmaps(const QPixmap *handleLabelArrowLeft,
                                   const QPixmap *handleLabelArrowRight,
                                   const QPixmap *handleLabelArrowUp,
                                   const QPixmap *handleLabelArrowDown);
    void setImages(const MScalableImage *background,
                   const MScalableImage *backgroundElapsed,
                   const MScalableImage *backgroundReceived,
                   const MScalableImage *backgroundVertical,
                   const MScalableImage *backgroundVerticalElapsed,
                   const MScalableImage *backgroundVerticalReceived);
    void setGrooveMargin(qreal thickness);
    void setGrooveThickness(qreal thickness);
    void setGrooveLength(qreal prefLength, qreal minLength, qreal maxLength);
    void setIndicatorMargin(qreal margin);

    void setHandleIndicatorArrowMargin(qreal margin);

    void setElapsedOffset(qreal offset);
    void setIndicatorOffset(qreal offset);

    void setIndicatorStyleName(const QString &stylename);

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
    void updateHandlePos();
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
    qreal grooveMargin;

    int minimum;
    int maximum;
    int value;

    int loadedContentMinimum;
    int loadedContentMaximum;
    bool showSeekBar;

    MSliderHandle *sliderHandle;
    MSliderHandleIndicator *sliderHandleIndicator;

    QRectF sliderBoundingRect;

    qreal preferredLength;
    qreal minimumLength;
    qreal maximumLength;
    qreal indicatorMargin;
    qreal elapsedOffset;
    qreal indicatorOffset;

    QString indicatorStyleName;

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

    QPropertyAnimation *positionAnimation;

    QElapsedTimer timeOnFeedback;
    QElapsedTimer timeOnMove;
    QPointF previousPosition;
    int previousValue;
    int feedbackPlayedFor;

    int pressTimerId;

    int valueWhenPressed;

    int position;

    void init(MSlider *contoller);

    void insertMinIndicatorToLayout();
    void insertMaxIndicatorToLayout();
    void removeMinIndicatorFromLayout();
    void removeMaxIndicatorFromLayout();

    void updateOrientation();
    bool isCollision(QGraphicsSceneMouseEvent *event) const;
    int updateValue(QGraphicsSceneMouseEvent *event);
    int adjustValue(int minimum, int value, int maximum, int steps);
    void updateSliderGroove();
    void updateSeekBar();
    void playSliderMoveFeedback(int newValue, const QPointF& newPosition);
    void updateMoveFeedbackData(int newValue);
    QPropertyAnimation* createPositionAnimation();

#ifdef UNIT_TEST
    friend class Ut_MSliderView;
#endif // UNIT_TEST
};

#endif
