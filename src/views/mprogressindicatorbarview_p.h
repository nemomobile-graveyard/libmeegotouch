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
class QPropertyAnimation;
class QTimer;

class MProgressIndicatorBarViewPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MProgressIndicatorBarView)

    QImage leftEndImage;
    QImage rightEndImage;
    QImage rightEndMask;
    QImage leftEndMask;

    QImage barMask;

    int leftWidth, rightWidth, top, bottom;

    bool textureTiled() const;
    void setupAnimation();

protected:
    MProgressIndicatorBarView *q_ptr;

public Q_SLOTS:
    void setPosition();

public:
    MProgressIndicatorBarViewPrivate();
    ~MProgressIndicatorBarViewPrivate();

    void resetBarComposition();
    void setupBarBody();
    void updateBarPosition();
    void createMaskOnGeometry();

    void compositeBarForUnknownDuration();
    void figureOutSizes();
    void buildAnimationCache();
    bool fullWidth() const;

    qreal getPosition();

    void animate(bool);
    void updateAnimation();

    MProgressIndicator *controller;

    float elementSize;
    int activeElementCount;
    int position;
    int width;
    bool paused;

    QTimer* timer;

    QImage barBody;

    QImage rightEnd;
    QImage leftEnd;

    QRectF rightEndRect;
    QRectF leftEndRect;

    QPixmap barComposition;
    QPainter* backgroundPainter;
    int previousValue;

    MScalableImage* scalableBarImage;

    QList<QImage*> animationCache;
    int animationCacheSize;

#ifdef M_UNIT_TEST
    M_UNIT_TEST;
#endif
};

#endif // MPROGRESSINDICATORBARVIEW_P_H
