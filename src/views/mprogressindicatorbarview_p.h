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

#ifndef MPROGRESSINDICATORBARVIEW_P_H
#define MPROGRESSINDICATORBARVIEW_P_H

#include <QObject>
#include "mprogressindicatorbarview.h"

class MStyle;
class MProgressIndicator;
class QPixmap;
class QPropertyAnimation;

class MProgressIndicatorBarViewPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MProgressIndicatorBarView)

    QImage leftEndImage;
    QImage rightEndImage;
    QPixmap rightEndMask;
    QPixmap leftEndMask;

    QPixmap barMask;

    int leftWidth, rightWidth, top, bottom;

protected:
    MProgressIndicatorBarView *q_ptr;

public:
    MProgressIndicatorBarViewPrivate();
    ~MProgressIndicatorBarViewPrivate();

    void resetBarComposition();
    void setupBarBody();
    void updateBarPosition();
    void createMaskOnGeometry();

    void compositeBarForUnknownDuration();
    void figureOutSizes();

    Q_PROPERTY(qreal position READ getPosition WRITE setPosition)

    qreal getPosition();
    void setPosition(qreal pos);

    void animate(bool);
    void updateAnimation();

    MProgressIndicator *controller;

    float elementSize;
    int activeElementCount;
    qreal position;
    QPropertyAnimation *animation;
    int width;

    QPixmap barBody;

    QPixmap rightEnd;
    QPixmap leftEnd;

    QRectF rightEndRect;
    QRectF leftEndRect;

    QPixmap barComposition;
    QPainter* backgroundPainter;
    int previousValue;

    MScalableImage* scalableBarImage;

#ifdef M_UNIT_TEST
    M_UNIT_TEST;
#endif
};

#endif // MPROGRESSINDICATORBARVIEW_P_H
