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
class MScalableImage;
class QPropertyAnimation;
class QTimer;

class MProgressIndicatorBarViewPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MProgressIndicatorBarView)

public:
    MProgressIndicatorBarViewPrivate();
    ~MProgressIndicatorBarViewPrivate();

    void drawBar(QPainter* painter) const;

    void clearBarImages();
    void clearAnimationCache();

    const QImage* getCurrentCachedImage() const;

    bool fullWidth() const;
    bool barImagesCreated() const;

    void animate(bool);
    void setupAnimation();
    void setupBarImages();

    const MWindow* getMWindow();

    MProgressIndicator *controller;

public Q_SLOTS:
    void setAnimationCacheIndex();
    void switcherEntered();
    void switcherExited();

protected:
    MProgressIndicatorBarView *q_ptr;

private:
    void createBarImages();
    void createAnimationCache();
    void drawComposedRectangle(QPainter* painter, const QRectF& rect) const;
    void drawTexture(QPainter* painter, const QRectF& rect) const;

    QTimer* animationTimer;

    QImage leftEndImage;
    QImage rightEndImage;
    QImage barBodyImage;

    bool inSwitcher;

    QList<QImage*> animationCache;
    int animationCacheIndex;

#ifdef UNIT_TEST
    friend class Ut_MProgressIndicatorBarView;
#endif

#ifdef M_UNIT_TEST
    M_UNIT_TEST;
#endif
};

#endif // MPROGRESSINDICATORBARVIEW_P_H
